
#include "Cylinder.h"

Cylinder::Cylinder() {
  //Default constructor, makes empty cylinder of zero size
  innerRadius = 0;
  outerRadius = 0;
  height = 0;
  for(int i=0; i < 3; i++)
    centerPosition[i] = 0;

}

Cylinder::Cylinder(Double_t setInnerRadius, Double_t setOuterRadius,
		   Double_t setHeight, Double_t setCenterPosition[]) {
  //Make a cylinder with center at setCenterPosition, 
  //that's a "ring" (with inner and outer radius) and has height, so the
  //top will be at z = setCenterPosition[2] + height/2
  //bottom will be at z = setCenterPosition[2] - height/2
  //Note that innerRadius <= 0 will be "solid cylinder" (i.e. no central hole)
  //As a consequence, suggested usage would be -1 for that situation

  innerRadius = setInnerRadius;
  outerRadius = setOuterRadius;
  height = setHeight;
  for(int i=0; i < 3; i++)
    centerPosition[i] = setCenterPosition[i];

}

void Cylinder::SetInnerRadius(Double_t setInnerRadius) {
  innerRadius = setInnerRadius;
}

void Cylinder::SetOuterRadius(Double_t setOuterRadius) {
  outerRadius = setOuterRadius;
}

void Cylinder::SetHeight(Double_t setHeight) {
  height = setHeight;
}

void Cylinder::SetCenterPosition(Double_t setCenterPosition[]) {
  for(int i=0; i < 3; i++)
    centerPosition[i] = setCenterPosition[i];
}

Double_t Cylinder::GetInnerRadius() {
  return innerRadius;
}

Double_t Cylinder::GetOuterRadius() {
  return outerRadius;
}

Double_t Cylinder::GetHeight() {
  return height;
}

void Cylinder::GetCenterPosition(Double_t getCenterPosition[]) {
  for(int i=0; i < 3; i++)
    getCenterPosition[i] = centerPosition[i];
}

Bool_t Cylinder::IsPointInside(Double_t position[]) {
  //This checks to see if the point give in cartesian coordinates
  //by position is inside the cylinder

  //Now convert to local cylindrical coordinates
  //Don't need phi right now, but keep it in case needed later
  Double_t localCyl[3]; //Order will be: r, phi, z
  ConvertToLocalCyl(position, localCyl);

  if(innerRadius > 0 && localCyl[0] < innerRadius) {
    //inside the inner radius, so false
    return false;
  }
  if(localCyl[0] > outerRadius) {
    //outside cylinder, so false
    return false;
  }
  if(localCyl[2] < -height/2) {
    //Below cylinder
    return false;
  }
  if(localCyl[2] > height/2) {
    //Above cylinder
    return false;
  }
  
  //passed all checks, must be inside cylinder
  return true;

}

Double_t Cylinder::FindCrossPoint(Double_t currentPosition[],
				 Double_t direction[],
				 Double_t crossPosition[]) {
  //This finds where, if at all, the line starting at currentPosition
  //and going in the direction of direction (hah, appropriate name)
  //intersects the boundary of the cylinder.  This position will
  //be copied to the array crossPosition, and the amount that direction
  //must be multiplied by will be returned - i.e. expect
  //crossPosition = currentPosition + amount*direction
  //If there is no intersection, or the intersection would be "behind"
  //(i.e on the line but in the wrong direction), crossPosition is
  //left unchanged and the return value is -1

  //Now, we just need to solve a couple of equations, basically check
  //whether each cylinder surface is intersected by the line and where
  //We'll do this by solving (vector) x_new = x_curr + direction*t
  //for the z position of the top and bottom, see if the r position is in the 
  //correct range, then repeat for the r position of the curved surfaces
  //and see if the z's are in the right range.  If multiple hits (possible),
  //pick the first one that occurs, i.e. the one with smallest t

  Double_t bestDistance = -1;
  Double_t distance = -1;
  
  //Switch to local coordinates
  Double_t localCart[3];
  Double_t r, z, x, y;
  for(int i=0; i < 3; i++)
    localCart[i] = currentPosition[i] - centerPosition[i];

  //First do top, solve z_curr + v_z*t = h/2
  if(TMath::Abs(direction[2]) > 1e-6) {
    //Ok, so has *some* z-velocity
    z = height/2;
    distance = (z - localCart[2])/direction[2];
    x = localCart[0] + distance*direction[0];
    y = localCart[1] + distance*direction[1];
    r = TMath::Sqrt(x*x + y*y);
    if(r > innerRadius && r < outerRadius && distance > 0) {
      //Hit!  If best set bestDistance to this.  Willfully neglecting edge
      if(bestDistance < 0 || distance < bestDistance)
	bestDistance = distance;
    }
  }
  
  //Bottom, solve z_curr + v_z*t = -h/2
  if(TMath::Abs(direction[2]) > 1e-6) {
    //Ok, so has *some* z-velocity
    z = -height/2;
    distance = (z - localCart[2])/direction[2];
    x = localCart[0] + distance*direction[0];
    y = localCart[1] + distance*direction[1];
    r = TMath::Sqrt(x*x + y*y);
    if(r > innerRadius && r < outerRadius && distance > 0) {
      //Hit!  If best set bestDistance to this.  Willfully neglecting edge
      if(bestDistance < 0 || distance < bestDistance)
	bestDistance = distance;
    }
  }

  if(innerRadius > 0) {
    //Inner cylindrical surface.  Need to solve 
    //(x_curr + v_x*t)^2 + (y_curr + v_y*t)^2 = r_inner^2
    //Ugh, quadratic equation time
    x = localCart[0];
    y = localCart[1];
    Double_t a = direction[0]*direction[0] + direction[1]*direction[1];
    Double_t b = 2*x*direction[0] + 2*y*direction[1];
    Double_t c = x*x + y*y - innerRadius*innerRadius;
    //Check discriminant
    Double_t discrim = b*b - 4*a*c;
    Double_t dist1 = -1, dist2 = -1; //Quadratic has two sols...
    if(discrim >= 0 && a > 1e-12) {
      dist1 = (-b + TMath::Sqrt(discrim))/(2*a);
      dist2 = (-b - TMath::Sqrt(discrim))/(2*a);
      //Pick first intersection
      if(dist1 > 0 && dist2 > 0) {
	if(dist1 < dist2) {
	  //Both positive, pick the smaller
	distance = dist1;
	} else {
	  distance = dist2;
	}
      } else {
	if(dist1 > 0) {
	  distance = dist1;
	} else if(dist2 > 0) {
	  distance = dist2;
	} else {
	  distance = -1;
      }
      }
    } else {
      distance = -1; //in case no solution
    }

    z = localCart[2] + direction[2]*distance;
    if(distance > 0 && z < height/2 && z > -height/2) {
      //Hit!  If best set bestDistance to this.  Willfully neglecting edge
      if(bestDistance < 0 || distance < bestDistance)
	bestDistance = distance; 
    }
  }

  //Repeat for outerRadius
  x = localCart[0];
  y = localCart[1];
  Double_t a = direction[0]*direction[0] + direction[1]*direction[1];
  Double_t b = 2*x*direction[0] + 2*y*direction[1];
  Double_t c = x*x + y*y - outerRadius*outerRadius;
  //Check discriminant
  Double_t discrim = b*b - 4*a*c;
  Double_t dist1 = -1;
  Double_t dist2 = -1; //Quadratic has two sols...
  if(discrim >= 0 && a > 1e-12) {
    dist1 = (-b + TMath::Sqrt(discrim))/(2*a);
    dist2 = (-b - TMath::Sqrt(discrim))/(2*a);
    //Pick first intersection
    if(dist1 > 0 && dist2 > 0) {
      if(dist1 < dist2) {
	//Both positive, pick the smaller
	distance = dist1;
      } else {
	distance = dist2;
      }
    } else {
      if(dist1 > 0) {
	distance = dist1;
      } else if(dist2 > 0) {
	distance = dist2;
      } else {
	distance = -1;
      }
    }
  } else {
    distance = -1; //in case no solution
  }
  if(distance < 0)
    distance = -1;

  z = localCart[2] + direction[2]*distance;
  if(distance > 0 && z < height/2 && z > -height/2) {
    //Hit!  If best set bestDistance to this.  Willfully neglecting edge
    if(bestDistance < 0 || distance < bestDistance)
      bestDistance = distance; 
  }

  //Ok, checked all surfaces.  If bestDistance set, fill up output vector
  if(bestDistance > 0) {
    for(int i=0; i < 3; i++)
      crossPosition[i] = currentPosition[i] + direction[i]*bestDistance;
  }

  return bestDistance;
}

void Cylinder::ConvertToLocalCyl(Double_t position[], Double_t localCyl[]) {
  
  Double_t relativePosition[3];
  for(int i=0; i < 3; i++)
    relativePosition[i] = position[i] - centerPosition[i];

  localCyl[0] = TMath::Sqrt(relativePosition[0]*relativePosition[0] +
			    relativePosition[1]*relativePosition[1]);
  localCyl[1] = TMath::ATan2(relativePosition[0], relativePosition[1]);
  localCyl[2] = relativePosition[2];

}
