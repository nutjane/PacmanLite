#define black           0 
#define darkblue        1 
#define darkgreen       2 
#define aqua            3 
#define darkred         4 
#define darkviolet      5 
#define darkyellow      6 
#define gray            7 
#define darkgray        8 
#define blue            9 
#define green           10 
#define cyan            11 
#define red             12 
#define violet          13 
#define yellow          14 
#define white           15 
#define color(txt,back) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), back*16+txt) 
#define reset() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7) 
