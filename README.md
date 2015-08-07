# PacmanLite

Pacman Lite is my first year student (freshmen) project of ITCS200 - Fundamental of Programming course. 
This project was written in C language.

## How it works

-	Stage (Background) :: [ purpose : to shop the pathway ] 
  - keep value of each position by 2-dimension array (ascii array)
-	Point (dot on pathway) :: [ purpose : will be kept by user]
  -  keep value of each position by 2-dimension array (int array [0,1])
-	Monster ::  [ purpose : to catch user ]
  - keep position of each monster in each variable and then by following steps …
    1. use if-function to find which direction that monster can go (check from stage array and monster position variable) 
    2.	keep position of each position that monster can go in other variable
    3.	use some fraction to find the length of that position and user position (old homework)  then compare the nearest point between each position that monster can go. (So monster can go in the best direction)
-	User (Pacman) ::  [ purpose : to keep all points on the pathway and run away from monsters ]
  - move by get arrow value from user (ascii check) 
-	Game score :: [ purpose : to show the score ]
  - score is increased by counting from Point array

## How it ends
-	All of the dots is gone -> WIN !
-	User (Pacman) is captured by any monster -> LOSE
