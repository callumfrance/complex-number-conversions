/*
 * =====================================================================================
 *       Filename:  conversions2.c
 *
 *    Description:  polar <= => rectangular and so much more
 *    				addition and subtraction
 *    				multiplication and division
 *
 *        Version:  1.2
 *        Created:  24/08/2016 21:17:25
 *       Compiler:  gcc
 *
 *         Author: Callum France 
 * =====================================================================================
 *
 *           _        _______  _______  _______  _
 * |\     /|( (    /|(  ____ )(  ____ \(  ___  )( \
 * | )   ( ||  \  ( || (    )|| (    \/| (   ) || (
 * | |   | ||   \ | || (____)|| (__    | (___) || |
 * | |   | || (\ \) ||     __)|  __)   |  ___  || |
 * | |   | || | \   || (\ (   | (      | (   ) || |
 * | (___) || )  \  || ) \ \__| (____/\| )   ( || (____/\
 * (_______)|/    )_)|/   \__/(_______/|/     \|(_______/
 *
 */


#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define PI 3.14159265
#define RAD 0.01745329		// = pi / 180

//	=====	Structure Declarations	=====

typedef struct{
	long double x;
	long double y;
}rectangular;

typedef struct{
	long double z;
	long double t;
	long double rad;
}polar;

//	=====	Function Declarations	=====

rectangular getRect();
polar getPol();
polar toPol(rectangular rectOG);
rectangular toRect(polar polOG);

rectangular ADD(rectangular rect1, rectangular rect2);
rectangular SUB(rectangular rect1, rectangular rect2);
rectangular MUL(rectangular rect1, rectangular rect2);
rectangular DIV(rectangular rect1, rectangular rect2);

//	=====	Main Function	=====

int main(void){
	char exit = 1;
	while( exit != 0 ){
		char type;
		rectangular unin[2];									//this unions values start off unknown but will become rectangular
		rectangular rectans;
		polar polans;

		printf("\nEnter command\nr\t for rectangular -> polar\np\t for polar -> rectangular\n+ -\t for addition/subtraction\n* /\t for multiplication/division\t]");
		// scanf("%c", &type);
		type = getchar();

		switch(type){
			case 'r' :
				{
					rectangular rectold = getRect();
					polar newpolar = toPol(rectold);
					printf("Polar value is \t\t\t\t[%Lg < %Lg deg\n", newpolar.z, newpolar.t);
					break;
				}
			case 'p' :
				{
					polar polold = getPol();
					rectangular newrect = toRect(polold);
					//printf("Rectangle value is \t\t\t\t[");					//print to look nice
					break;
				}
			case '*' : case '-' : case '+' : case '/' :

				for( int i = 0; i < 2; i++ ){
					char ty;
					long double in1, in2;
					printf("\nEnter value type followed by value\n(e.g. p 15.2 95.1)\n\t]");
					scanf("%c %Lf %Lf", &ty, &in1, &in2);
					getchar();
					
					switch( ty ){								//makes sense of input
						case 'r' :
							unin[i].x = in1;
							unin[i].y = in2;

							break;
						case 'p' :								//convert to rectangular as well
							{
								polar temp;
								temp.z = in1;
								temp.t = in2;

								unin[i] = toRect(temp);

								break;
							}
					}
				}
				

				break;
		}
		switch(type){						//seperate into each operator to perform correct operation
			case '+' :
				rectans = ADD(unin[0], unin[1]);

				break;
			case '-' :
				rectans = SUB(unin[0], unin[1]);

				break;
			case '*' :
				rectans = MUL(unin[0], unin[1]);

				break;
			case '/' :
				rectans = DIV(unin[0], unin[1]);

				break;
		}
		switch(type){
			case '+' : case '-' : case '*' : case '/' :		//merge operator for printing
				polans = toPol( rectans );
				long double yabs = fabsl( rectans.y );

				printf("Polar value is \t\t\t\t[%Lg < %Lg deg\n", polans.z, polans.t);
				printf("Rectangular value is \t\t\t[");
				if( rectans.y < 0 ){						//nice rectangular printing technique
					printf("%Lg - j%Lg\n", rectans.x, yabs);
				}else{
					printf("%Lg + j%Lg\n", rectans.x, yabs);
				}	
				break;
		}



	}
	return(0);
}

rectangular	getRect(){
	rectangular rect;
	printf("Enter rectangular value:\t\t\t]");
	scanf("%Lf %Lf", &rect.x, &rect.y);
	return(rect);
}

polar getPol(){
	polar pol;
	printf("Enter polar value:\t\t\t\t]");
	scanf("%Lf %Lf", &pol.z, &pol.t);

	char posadj = 0;

	while( pol.t > 360 ){					//ensures angle is a unit circle
		pol.t = pol.t - 360;
		posadj = 1;
	}
	while( pol.t < -360 ){
		pol.t = pol.t + 360;
		posadj = 1;
	}
	if( (pol.t > 180) && (pol.t < 360) ){		//ensure angle is within +-180
		pol.t = pol.t - 360;
		posadj = 1;
	}else if( (pol.t < -180) && (pol.t > -360) ){
		pol.t = 360 - pol.t;
		posadj = 1;
	}
	
	pol.rad = pol.t * PI / 180;				//calculate radians component
	

	if( posadj == 1 ){						//show that angle has been adjusted for calculations
		printf("Angle adjusted\n");
	}
	if( pol.z < 0 ){
		printf("You can't have a magnitude < 0\n");
	}
	return(pol);
}

polar toPol(rectangular rectOG){			//take a rectangular value => polar
	polar pans;
	long double xabs, yabs, sqz;
	double val = 180 / PI;

	xabs = fabsl( rectOG.x );
	yabs = fabsl( rectOG.y );
	sqz = (xabs * xabs) + (yabs * yabs);
	pans.z = sqrt( sqz );

	pans.rad = atan2(rectOG.y, rectOG.x);	//ensures phase is in correct quadrant
	pans.t = pans.rad * val;


	printf("%Lg, %Lg deg, %Lg rad\n", pans.z, pans.t, pans.rad);

	return(pans);
}

rectangular toRect(polar polOG){			//take a polar value => rectangular
	rectangular rans;
	
	long double sint = sin( polOG.t * RAD );
	long double cost = cos(polOG.t * RAD );
	rans.x = polOG.z * cost;
	rans.y = polOG.z * sint;

	long double yabs = fabsl( rans.y );
	if( rans.y < 0 ){						//nice rectangular printing technique
		printf("%Lg - j%Lg\n", rans.x, yabs);
	}else{
		printf("%Lg + j%Lg\n", rans.x, yabs);
	}	

	return(rans);	
}

//	=====	Operations	=====

rectangular ADD(rectangular rect1, rectangular rect2){
//			(a + jb) + (c + jd) = (a + c)+j(b + d)
	rectangular addans;

	addans.x = rect1.x + rect2.x;
	addans.y = rect1.y + rect2.y;

	return(addans);
}

rectangular SUB(rectangular rect1, rectangular rect2){
//			(a + jb) - (c + jd) = (a - c)+j(b - d)
	rectangular subans;

	subans.x = rect1.x - rect2.x;
	subans.y = rect1.y - rect2.y;

	return(subans);
}

rectangular MUL(rectangular rect1, rectangular rect2){
//			(a + jb) * (c + jd) = (ac - bd)+j(bc + ad)
	rectangular mulans;

	mulans.x = (rect1.x * rect2.x) - (rect1.y * rect2.y);
	mulans.y = (rect1.y * rect2.x) + (rect1.x * rect2.y);

	return(mulans);
}

rectangular DIV(rectangular rect1, rectangular rect2){
//			(a + jb) / (c + jd) = ((ac + bd) / (c^2 + d^2))+j((bc - ad)/(c^2 + d^2))
	rectangular divans;
	long double squares, realn, imn;

	if( (rect2.x != 0) | (rect2.y != 0) ){
		squares = (rect2.x * rect2.x) + (rect2.y * rect2.y);	//denominator
		realn = (rect1.x * rect2.x) + (rect1.y * rect2.y);
		imn = (rect1.y * rect2.x) - (rect1.x * rect2.y);		//numerators

		divans.x = (long double) realn / squares;
		divans.y = (long double) imn / squares;

	}else{
		printf("\tINVALID\n\tThe second complex number has a zero value.\n\n");
	}

	return(divans);
// only accurate to fifth decimal place!!!!!
}
