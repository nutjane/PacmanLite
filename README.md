# PacmanLite

Pacman Lite is my first year student (freshmen) project of ITCS200 - Fundamental of Programming course. 
This project was written in C language.

## How it works

-	Stage (Background) :: [ purpose : to stop the pathway ] 
  - keep value of each position in 2-dimension array (ascii array)
-	Point (dot on the pathway) :: [ purpose : be collected by pacman]
  -  keep value of each position in 2-dimension array (int array [0,1])
-	Monster ::  [ purpose : to catch pacman ]
  - keep position of each monster in each variable and then follow these steps …
    1. use conditional-function to find which direction that monster can go (check from bacground array and monster position variable) 
    2.	keep position of each location that monster can go in other variable
    3.	use some fraction to calculate the length of each position then compare the nearest point between each position that monster can go. (So monster will go in the best direction)
-	User (Pacman) ::  [ purpose : to collect every point on the pathway and run away from monsters ]
  - move by get arrow value from user input 
-	Game score :: [ purpose : to show the score ]
  - score is increased by collecting points from Point array

## How it ends
-	All of the dots is gone -> WIN !
-	User (Pacman) is captured by any monster -> LOSE
