/*
*********************************************Pokemon - Win32 Console Game***********************************************
Institutional Workshop Project
Developed by-
    Tolesh Pathak   20BCS3521
    Abhay Tomer     20BCS3566
    Gautam Sharma   20BCS3548
*/
#include<iostream>
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
using namespace std;

/*Order of Pokemon in our game is as follows
1. Bulabasaur
2. Charmander
3. Squirtle
4. Pikachu
5. Pidgey
6. Electabuzz
7. Vulpix
8. Ratata
9. Seel
10. Bellsprout
*/

/*
To be noted:
While displaying ascii art of pokemon these limits would be crossed as per convinience
*/
#define Vsize 30//the vertical screen limit of application
#define Hsize 120//the horizontal screen limit of application

HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorposition;
//this function positions the cursor on entered coordinates.
void gotoxy(int x, int y){
  cursorposition.X=x;
  cursorposition.Y=y;
  SetConsoleCursorPosition(console, cursorposition);
}

//this function causes the system to wait for given amount of milliseconds
void delay(int delay){
  clock_t start;
  start=clock();
  while(clock()-start<delay){}
}

//the following functions and datatypes handle the database for pokemon in both pokedex and gameplay
//the following datatype will help to sequence damage etc in battles
typedef struct POKEMONATTACKS{
  char attackname[20];//it will be store the name of the attack
  int damagebyattackdenominator;//it will store the damage range of the attack
} POKEMONATTACKS;
//the following datatype will define all the values related to a pokemon
typedef struct POKEMON{
  int pokedexnumber;//here the index number of the pokemon will be stored
  char name[20];//here the name of the pokemon will be stored
  char type[10];//here the type of the pokemon will be stored
  string INFO[2];//here the general information for the pokemon will be stored(mainly facts from pokemon.com)
  POKEMONATTACKS attacks[4];//here an array of attacks will be stored
  int health=90;//all pokemons are given basic health of 60. if it falls to 0, the pokemon will faint
} POKEMON;
//the entire database of pokemon for our game is as follows
POKEMON POKEDEXDATABASE[10]={
  {1, "Bulbasaur",  "Grass",
  {"There is a plant seed on its back right from the day this Pokemon is born.","The seed slowly grows larger."},
  {{"Seed Bomb",16},  {"Wild Wine",12}, {"Razor Leaves",8}, {"Tackle",4}}},
  {2, "Charmander", "Fire",
  {"From the time it is born, a flame burns at the tip of its tail.","Its life would end if the flame were to go out."},
  {{"Flamethrower", 16},  {"Amber",12}, {"Tail Spin",8},  {"Scratch",4}}},
  {3, "Squirtle",  "Water",
  {"When it feels threatened, it draws its limbs inside its shell","and sprays water from mouth."},
  {{"Water Canon",16},  {"Shell Attack",12},  {"Bubble",8}, {"Scratch",4}}},
  {4, "Pikachu",  "Electric",
  {"Pikachu that can generate powerful electricity have cheek sacs","that are extra soft and super stretchy."},
  {{"Thuderbolt",16}, {"Iron Tail",12}, {"Sparkles",8}, {"Tackle",4}}},
  {5, "Pidgey", "Normal",
  {"Very docile.","If attacked, will often kick sand to protect itself rather than fight back."},
  {{"Aerial Ace", 16},  {"Whirlwind",12}, {"Air Slash", 8}, {"Peck Attack",4}}},
  {6, "Electabuzz", "Electric",
  {"Many power plants keep Ground-type Pokemon around as a defense","against Electabuzz that come seeking electricity."},
  {{"Thunder Punch",16},  {"Karate Chop",12}, {"Sparkles",8}, {"Static",4}}},
  {7, "Vulpix", "Fire",
  {"While young, it has six gorgeous tails.","When it grows, several new tails are sprouted."},
  {{"Fire",16}, {"Gold Tackle",12}, {"Flare",8},  {"Gnaw",4}}},
  {8, "Ratata", "Normal",
  {"Will chew on anything with its fangs.","If you see one, you can be certain that 40 more live in the area."},
  {{"Spray Poison",16}, {"Paralyze",14},  {"Venom Bite", 8},  {"Gnaw",4}}},
  {9, "Seel", "Water",
  {"Loves freezing-cold conditions.","Relishes swimming in a frigid climate of around 14 degrees Fahrenheit."},
  {{"Ice Beam",16}, {"Horn Attack",12}, {"Tail Spin", 8}, {"Lick", 4}}},
  {10,  "Bellsprout",  "Grass",
  {"Prefers hot and humid places.","It ensnares tiny bugs with its vines and devours them."},
  {{"Solar Beam",16}, {"Poison Powder", 12},  {"Razor Leaves", 8},  {"Tackle",4}}}
};

//map for story mode
#define VmapSize 20
#define HmapSize 30
char worldmapstorymode[VmapSize][HmapSize]={
  "#############################",//0
  "##Vermillion#################",//1
  "###########@          #######",//2
  "#################     #######",//3
  "##                         ##",//4
  "##                         ##",//5
  "##   ###################   ##",//6
  "##   ########Petwer#####   ##",//7
  "##   #############&        ##",//8
  "##          #################",//9
  "##          ###Cerulean######",//10
  "#######     ##### $ #########",//11
  "#######     ###### ##########",//12
  "##                         ##",//13
  "##                         ##",//14
  "#######################    ##",//15
  "##Pallet Town##########    ##",//16
  "##############             ##",//17
  "##Start:#X                 ##",//18
  "#############################",//19
};

//the following datatype will define all the values related to a trainer
//the following will assist in arcade mode
typedef struct ARCADETRAINER{
  char name[20];
  POKEMON Pokemon;
} ARCADETRAINER;
//this will keep count of wins in arcade mode
static int USERarcadeWINcounter=0;
static int STORYPlayerGYMBadgecount=0;

//main application functions start from here

//all the function related to console ui and main menu of the game are as follows
void bootload();
void printtitlepokemon();
void mainmenuborder();
void mainmenucontent();

//all the functions related to story mode are as follows
void storymodeapplicationlaunch();
void ceruleangymfight(ARCADETRAINER);
void petwergymfight(ARCADETRAINER);
void vermilliongymfight(ARCADETRAINER);
void GYMBattle(ARCADETRAINER,ARCADETRAINER);

//all the functions related to arcade mode are as follows
void arcademodeapplicationlaunch();
int arcadebattle(ARCADETRAINER,ARCADETRAINER);

//all the functions related to pokedex menu are as follows
void pokedexapplicationlaunch();
void pokemonasciiartprinter(int);
void pokedexdisplaydetails(int);

//the following fuction will be for exit related protocols
void exitornotcontent();

//#INTMAINISHERE
//added this to make it easy to search
int main(){
  srand(time(0));//this will help us to generate truly random numbers
  system("Color 0A");//here the console text color is changed to green
  bootload();//here the function for booting screen is called
  backtomainmenu://from this point the main menu starts
  system("Color 06");//here the console text color is changed to yellow
  printtitlepokemon();//prints the "pokemon" ascii art
  delay(100);
  mainmenuborder();//prints a border around the edges
  delay(100);
  mainmenucontent();//displays the options in main menu
  int mainmenuselection;
  cin>>mainmenuselection;//takes an input from user about which part of game they want to explore
  switch(mainmenuselection){
    case 1:
      storymodeapplicationlaunch();
      goto backtomainmenu;//this will jump back the user to main menu
      break;
    case 2:
      arcademodeapplicationlaunch();//this will launch the arcade mode
      goto backtomainmenu;//this will jump back the user to main menu
      break;
    case 3:
      pokedexapplicationlaunch();//this will launch the pokedex
      goto backtomainmenu;//this will jump back the user to main menu
      break;
    case 4:
      exitornotcontent();//this will print the exit screen content
      char exitornot;
      cin>>exitornot;
      if(exitornot=='n'||exitornot=='N'){goto backtomainmenu;}//this will jump back the user to main menu
      break;
  }
  return 0;
}

void GYMBattle(ARCADETRAINER USER, ARCADETRAINER CPU){
  pointforrematch://to this point code will be redirected in case of rematch
  //reinstating the health levels to maximum
  USER.Pokemon.health=80;
  CPU.Pokemon.health=80;
  //game battle loop
  //will run till one of the pokemon faints
  while(USER.Pokemon.health>0&&CPU.Pokemon.health>0){
    beginingofthisloop:
    system("cls");
    mainmenuborder();
    gotoxy(56,2);cout<<"Battle";
    gotoxy(15,6);cout<<USER.name;
    gotoxy(15,8);cout<<USER.name<<"'s "<<USER.Pokemon.name;
    gotoxy(15,10);cout<<"Health: "<<USER.Pokemon.health;
    gotoxy(15,12);cout<<"Attacks: ";
    int gotoxyattacklister1=13;
    for(int i=0;i<4;i++){
      gotoxy(17,gotoxyattacklister1);
      cout<<i+1<<" "<<USER.Pokemon.attacks[i].attackname;
      gotoxyattacklister1+=1;
    }
    gotoxy(75,6);cout<<CPU.name;
    gotoxy(75,8);cout<<CPU.name<<"'s "<<CPU.Pokemon.name;
    gotoxy(75,10);cout<<"Health: "<<CPU.Pokemon.health;
    gotoxy(75,12);cout<<"Attacks: ";
    int gotoxyattacklister2=13;
    for(int i=0;i<4;i++){
      gotoxy(77,gotoxyattacklister2);
      cout<<i+1<<" "<<CPU.Pokemon.attacks[i].attackname;
      gotoxyattacklister2+=1;
    }
    gotoxy(15,18);cout<<"Choose Your Attack (Enter 0 to exit Gym): ";
    int userattackinput;
    cin>>userattackinput;
    if(userattackinput<0||userattackinput>4){goto beginingofthisloop;}
    else if(userattackinput==0){return;}
    userattackinput--;
    int damagetocpu=0, damagetouser=0;
    damagetocpu=rand()%USER.Pokemon.attacks[userattackinput].damagebyattackdenominator;
    if(damagetocpu<(USER.Pokemon.attacks[userattackinput].damagebyattackdenominator/2)){
      damagetocpu+=(USER.Pokemon.attacks[userattackinput].damagebyattackdenominator/2);
    }
    CPU.Pokemon.health-=damagetocpu;
    int cpuattackinput=rand()%4;
    damagetouser=rand()%CPU.Pokemon.attacks[cpuattackinput].damagebyattackdenominator;
    if(damagetouser<(CPU.Pokemon.attacks[cpuattackinput].damagebyattackdenominator/2)){
      damagetouser+=(CPU.Pokemon.attacks[cpuattackinput].damagebyattackdenominator/2);
    }
    USER.Pokemon.health-=damagetouser;
    gotoxy(15, 21);cout<<"Your "<<USER.Pokemon.name<<" used "<<USER.Pokemon.attacks[userattackinput].attackname;
    delay(2000);
    gotoxy(15, 23);cout<<CPU.name<<"'s "<<CPU.Pokemon.name<<" used "<<CPU.Pokemon.attacks[cpuattackinput].attackname;
    delay(2000);
    if(CPU.Pokemon.health<=0){
      gotoxy(15,25);
      cout<<CPU.name<<"'s "<<CPU.Pokemon.name<<" fainted.";
    }
    else if(USER.Pokemon.health<=0){
      gotoxy(15,25);
      cout<<USER.name<<"'s "<<USER.Pokemon.name<<" fainted.";
    }
  }
  delay(2000);
  system("cls");
  mainmenuborder();
  gotoxy(55,14);
  if(CPU.Pokemon.health<=0){
    cout<<"YOU WIN!";STORYPlayerGYMBadgecount++;
    delay(500);system("Color B6");
    delay(500);system("Color C6");
    delay(500);system("Color D6");
    delay(500);system("Color E6");
    delay(500);
  }
  else{cout<<"YOU LOSE";}
  system("Color 06");
  delay(2500);
  invalidinputpoint:
  gotoxy(36,16);
  cout<<"Enter 1 for a rematch, 0 to go back to map: ";
  int userinputatarcadeend;
  cin>>userinputatarcadeend;
  if(userinputatarcadeend==1){goto pointforrematch;}
  else if(userinputatarcadeend==0){return;}
  else{goto invalidinputpoint;}
}

void vermilliongymfight(ARCADETRAINER Player){
  ARCADETRAINER GYMleader={"Lt. Surge",POKEDEXDATABASE[5]};
  system("cls");
  mainmenuborder();
  delay(300);
  gotoxy(44,13);cout<<"WELCOME TO THE VERMILLION GYM";
  delay(1000);
  gotoxy(37,15);cout<<"Hi! I am Lt. Surge and am this GYM's Leader.";
  delay(1000);
  gotoxy(47,17);cout<<"You will battle me now.";
  delay(3000);
  GYMBattle(Player,GYMleader);
}

void petwergymfight(ARCADETRAINER Player){
  ARCADETRAINER GYMleader={"Brock",POKEDEXDATABASE[6]};
  system("cls");
  mainmenuborder();
  delay(300);
  gotoxy(46,13);cout<<"WELCOME TO THE PETWER GYM";
  delay(1000);
  gotoxy(39,15);cout<<"Hi! I am Brock and am this GYM's Leader.";
  delay(1000);
  gotoxy(47,17);cout<<"You will battle me now.";
  delay(3000);
  GYMBattle(Player,GYMleader);
}

void ceruleangymfight(ARCADETRAINER Player){
  ARCADETRAINER GYMleader={"Misty",POKEDEXDATABASE[8]};
  system("cls");
  mainmenuborder();
  delay(300);
  gotoxy(45,13);cout<<"WELCOME TO THE CERULEAN GYM";
  delay(1000);
  gotoxy(39,15);cout<<"Hi! I am Misty and am this GYM's Leader.";
  delay(1000);
  gotoxy(47,17);cout<<"You will battle me now.";
  delay(3000);
  GYMBattle(Player,GYMleader);
}

//this function will run the storymode
void storymodeapplicationlaunch(){
  system("cls");
  system("Color 06");
  mainmenuborder();
  gotoxy(48,2);cout<<"Welcome to Story Mode";
  gotoxy(15,6);cout<<"Please Enter Your Name: ";
  ARCADETRAINER Player;cin>>Player.name;
  gotoxy(39,10);cout<<"Select Your Starter Pokemon";
  gotoxy(15,12);cout<<"1. Bulbasaur";
  gotoxy(45,12);cout<<"2. Charmander";
  gotoxy(75,12);cout<<"3. Squirtle";
  gotoxy(50,15);cout<<"->";
  int playerstarterpokemonselection;
  cin>>playerstarterpokemonselection;playerstarterpokemonselection--;
  Player.Pokemon=POKEDEXDATABASE[playerstarterpokemonselection];
  gotoxy(15,17);cout<<"You have selected "<<Player.Pokemon.name<<"!";
  delay(2000);
  system("cls");
  mainmenuborder();
  delay(500);
  gotoxy(43,14);cout<<"WELCOME TO THE WORLD OF POKEMON!";
  delay(1000);
  gotoxy(14, 16);cout<<"Defeat all the 3 gym leaders and earn their badges in order to become the Pokemon Master.";
  delay(5000);
  while(STORYPlayerGYMBadgecount<3){
    system("cls");
    for(int y=0;y<VmapSize;y++){cout<<worldmapstorymode[y]<<endl;}
    cout<<"GYM Badges Collected: "<<STORYPlayerGYMBadgecount<<endl;
    for(int y=0;y<VmapSize;y++){
      for(int x=0;x<HmapSize;x++){
        switch(worldmapstorymode[y][x]){
          case '#':{
            worldmapstorymode[y][x]=219;
          }break;
          case 'X':{
            if(GetAsyncKeyState(VK_UP)!=0){
              int y_temp=(y-1);
              switch(worldmapstorymode[y_temp][x]){
                case ' ':{
                  worldmapstorymode[y][x]=' ';
                  y-=1;
                  worldmapstorymode[y_temp][x]='X';
                }break;
                case '$':{
                  ceruleangymfight(Player);
                }break;
              }
            }
            if(GetAsyncKeyState(VK_DOWN)!=0){
              int y_temp=(y+1);
              switch(worldmapstorymode[y_temp][x]){
                case ' ':{
                  worldmapstorymode[y][x]=' ';
                  y+=1;
                  worldmapstorymode[y_temp][x]='X';
                }break;
              }
            }
            if(GetAsyncKeyState(VK_LEFT)!=0){
              int x_temp=(x-1);
              switch(worldmapstorymode[y][x_temp]){
                case ' ':{
                  worldmapstorymode[y][x]=' ';
                  x-=1;
                  worldmapstorymode[y][x_temp]='X';
                }break;
                case '&':{
                  petwergymfight(Player);
                }break;
                case '@':{
                  vermilliongymfight(Player);
                }break;
              }
            }
            if(GetAsyncKeyState(VK_RIGHT)!=0){
              int x_temp=(x+1);
              switch(worldmapstorymode[y][x_temp]){
                case ' ':{
                  worldmapstorymode[y][x]=' ';
                  x+=1;
                  worldmapstorymode[y][x_temp]='X';
                }break;
              }
            }
          }
        }
      }
    }
  }
  system("cls");
  mainmenuborder();
  delay(500);
  gotoxy(50,13);cout<<"Congratulations!";
  delay(1000);
  gotoxy(42,15);cout<<"You have completed the Story Mode";
  delay(1000);
  gotoxy(44,17);cout<<"You are a Pokemon Master Now";
  delay(500);system("Color B5");
  delay(500);system("Color C5");
  delay(500);system("Color D5");
  delay(500);system("Color E5");
  delay(500);system("Color 05");
  delay(4000);
}

//this function will run the battle between user and cpu
int arcadebattle(ARCADETRAINER USER, ARCADETRAINER CPU){
  pointforrematch://to this point code will be redirected in case of rematch
  //reinstating the health levels to maximum
  USER.Pokemon.health=80;
  CPU.Pokemon.health=80;
  //arcade game loop
  //will run till one of the pokemon faints
  while(USER.Pokemon.health>0&&CPU.Pokemon.health>0){
    beginingofthisloop:
    system("cls");
    mainmenuborder();
    gotoxy(56,2);cout<<"Battle";
    gotoxy(15,6);cout<<USER.name;
    gotoxy(15,8);cout<<USER.name<<"'s "<<USER.Pokemon.name;
    gotoxy(15,10);cout<<"Health: "<<USER.Pokemon.health;
    gotoxy(15,12);cout<<"Attacks: ";
    int gotoxyattacklister1=13;
    for(int i=0;i<4;i++){
      gotoxy(17,gotoxyattacklister1);
      cout<<i+1<<" "<<USER.Pokemon.attacks[i].attackname;
      gotoxyattacklister1+=1;
    }
    gotoxy(75,6);cout<<CPU.name;
    gotoxy(75,8);cout<<CPU.name<<"'s "<<CPU.Pokemon.name;
    gotoxy(75,10);cout<<"Health: "<<CPU.Pokemon.health;
    gotoxy(75,12);cout<<"Attacks: ";
    int gotoxyattacklister2=13;
    for(int i=0;i<4;i++){
      gotoxy(77,gotoxyattacklister2);
      cout<<i+1<<" "<<CPU.Pokemon.attacks[i].attackname;
      gotoxyattacklister2+=1;
    }
    gotoxy(15,18);cout<<"Choose Your Attack (Enter 0 to exit Arcade): ";
    int userattackinput=0;
    cin>>userattackinput;
    if(userattackinput<0||userattackinput>4){goto beginingofthisloop;}
    else if(userattackinput==0){return 0;}
    userattackinput--;
    int damagetocpu=0, damagetouser=0;
    damagetocpu=rand()%USER.Pokemon.attacks[userattackinput].damagebyattackdenominator;
    if(damagetocpu<(USER.Pokemon.attacks[userattackinput].damagebyattackdenominator/2)){
      damagetocpu+=(USER.Pokemon.attacks[userattackinput].damagebyattackdenominator/2);
    }
    CPU.Pokemon.health-=damagetocpu;
    int cpuattackinput=rand()%4;
    damagetouser=rand()%CPU.Pokemon.attacks[cpuattackinput].damagebyattackdenominator;
    if(damagetouser<(CPU.Pokemon.attacks[cpuattackinput].damagebyattackdenominator/2)){
      damagetouser+=(CPU.Pokemon.attacks[cpuattackinput].damagebyattackdenominator/2);
    }
    USER.Pokemon.health-=damagetouser;
    gotoxy(15, 21);cout<<"Your "<<USER.Pokemon.name<<" used "<<USER.Pokemon.attacks[userattackinput].attackname;
    delay(2000);
    gotoxy(15, 23);cout<<"CPU's "<<CPU.Pokemon.name<<" used "<<CPU.Pokemon.attacks[cpuattackinput].attackname;
    delay(2000);
    if(CPU.Pokemon.health<=0){
      gotoxy(15,25);
      cout<<CPU.name<<"'s "<<CPU.Pokemon.name<<" fainted.";
    }
    else if(USER.Pokemon.health<=0){
      gotoxy(15,25);
      cout<<USER.name<<"'s "<<USER.Pokemon.name<<" fainted.";
    }
  }
  delay(2000);
  system("cls");
  mainmenuborder();
  gotoxy(55,14);
  if(CPU.Pokemon.health<=0){
    cout<<"YOU WIN!";USERarcadeWINcounter++;
    delay(500);system("Color B9");
    delay(500);system("Color C9");
    delay(500);system("Color D9");
    delay(500);system("Color E9");
    delay(500);
  }
  else{cout<<"YOU LOSE";}
  system("Color 09");
  delay(2500);
  invalidinputpoint:
  gotoxy(20,16);
  cout<<"Enter 1 for a rematch, 0 to go back to main menu, 2 to go back to arcade menu: ";
  int userinputatarcadeend;
  cin>>userinputatarcadeend;
  if(userinputatarcadeend==1){goto pointforrematch;}
  else if(userinputatarcadeend==2){return 2;}
  else if(userinputatarcadeend==0){return 0;}
  else{goto invalidinputpoint;}
}

//this function will provide the setup menu for arcade mode
void arcademodeapplicationlaunch(){
  beginingofarcademode:
  system("cls");
  system("Color 09");
  mainmenuborder();
  gotoxy(53, 2);cout<<"Arcade Mode.";
  gotoxy(100, 2);cout<<"Wins: "<<USERarcadeWINcounter;
  int indexerforloopgotoxy=5;
  gotoxy(15, indexerforloopgotoxy);
  for(int i=0;i<10;i++){
    delay(100);
    cout<<i+1<<". "<<POKEDEXDATABASE[i].name<<"\t";
    if(i==8){cout<<"\t";}
    if((i==3||i==7||i==9)&&i!=0){indexerforloopgotoxy+=2; gotoxy(15, indexerforloopgotoxy);}
  }
  cout<<"Please Select Your Pokemon: ";
  int arcadeuserpokemon;
  cin>>arcadeuserpokemon;
  arcadeuserpokemon-=1;
  ARCADETRAINER USER={"USER"}, CPU={"CPU"};
  USER.Pokemon=POKEDEXDATABASE[arcadeuserpokemon];
  int arcadecpupokemon=rand()%10;
  CPU.Pokemon=POKEDEXDATABASE[arcadecpupokemon];
  gotoxy(15, indexerforloopgotoxy+3);
  cout<<"You have selected "<<POKEDEXDATABASE[arcadeuserpokemon].name<<"!";
  gotoxy(15, indexerforloopgotoxy+6);
  delay(2000);
  cout<<"Your opponent has selected "<<CPU.Pokemon.name<<"!";
  delay(2000);
  gotoxy(15, indexerforloopgotoxy+9);
  cout<<"Your Battle will start in a moment.";
  delay(2000);
  int continuityfromarcade=arcadebattle(USER,CPU);
  if(continuityfromarcade==2){goto beginingofarcademode;}
  else if(continuityfromarcade==0){return;}
}

//this function will display the details of the pokemon selected in the pokedex
void pokedexdisplaydetails(int indexnumber){
  indexnumber--;//here the number entered by user is decreased to match database
  beginingofpokedexdisplaydetails://here this segment's UI starts
  system("cls");
  mainmenuborder();//a border on edges is printed
  gotoxy(17,4);
  cout<<POKEDEXDATABASE[indexnumber].name;//pokemon's name is displayed
  gotoxy(17,6);
  cout<<"Index Number: "<<indexnumber+1;//pokemon's index number is displayed
  gotoxy(17,8);
  cout<<"Type: "<<POKEDEXDATABASE[indexnumber].type;//pokemon's type is displayed
  gotoxy(15,12);
  cout<<"Attacks:";//pokemon's attacks are displayed
  int indexerforgotoxyhere=14;
  for(int i=0;i<4;i++){
    gotoxy(18,indexerforgotoxyhere);
    cout<<POKEDEXDATABASE[indexnumber].attacks[i].attackname;//individual attack names
    indexerforgotoxyhere=indexerforgotoxyhere+2;
  }
  gotoxy(43,10);
  cout<<POKEDEXDATABASE[indexnumber].INFO[0];//pokemon's first information string is displayed
  gotoxy(43,11);
  cout<<POKEDEXDATABASE[indexnumber].INFO[1];//pokemon's second information string is displayed
  gotoxy(15,27);
  cout<<"Press 1 to see its ASCII art or Press 0 to go back to Pokedex: ";
  int pokedexdispselector;
  cin>>pokedexdispselector;
  if(pokedexdispselector==1){
    pokemonasciiartprinter(indexnumber);//function call is made to print ascii art of the pokemon
  }
  else if(pokedexdispselector==0){
    pokedexapplicationlaunch();//redirects the user back to pokedex
  }
  //the following sequence will take place if user enters an invalid input
  else{
    system("cls");
    mainmenuborder();
    gotoxy(44, 14);cout<<"Enter correct number in index.";
    delay(3000);
    goto beginingofpokedexdisplaydetails;//take back the user to begining of pokedex display function
  }
}

//this function launches the pokedex segment of this game
void pokedexapplicationlaunch(){
  relaunchpokedexapplication://the begining of pokedex UI
  system("cls");
  system("Color 04");//changes the console text color to red
  mainmenuborder();//a border is printed on the edges of the screen
  gotoxy(55, 2);cout<<"Pokedex.";//title
  int gotoindexer=5;
  //all the pokemon names along with their index number are displayed
  for(int i=0;i<10;i++){
    gotoxy(15, gotoindexer);
    cout<<POKEDEXDATABASE[i].pokedexnumber<<"\t"<<POKEDEXDATABASE[i].name;
    gotoindexer=gotoindexer+2;
    delay(100);
  }
  gotoxy(15, gotoindexer+2);
  cout<<"Enter the Index Number of a Pokemon to know about it (Enter 0 to go back): ";
  int pokedexselectioninput;
  cin>>pokedexselectioninput;
  if(pokedexselectioninput>0&&pokedexselectioninput<=10){
    pokedexdisplaydetails(pokedexselectioninput);//displays the information of the selected pokemon
  }
  else if(pokedexselectioninput==0){
    return;//this will redirect the user back to main menu
  }
  //sequence for invalid input will be as follows
  else{
    system("cls");
    mainmenuborder();
    gotoxy(44, 14);cout<<"Enter correct number in index.";
    delay(3000);
    goto relaunchpokedexapplication;//redirects the user back to begining of pokedex application
  }
}

//this function displays the whole loading sequence of first time game boot
void bootload(){
  system("cls");
  gotoxy(42, 1);cout<<"Pokemon is a trademark of Nintendo.";
  gotoxy(31, 3);cout<<"This game has been developed for Educational Puposes only.";
  gotoxy(22, 5);cout<<"The developer of this game strictly prohibits its use for comercial purposes.";
  gotoxy(26, 9);cout<<"This game has been developed by students of Chandigarh University.";
  gotoxy(46, 11);cout<<"Tolesh Pathak - 20BCS3521";
  gotoxy(47, 13);cout<<"Abhay Tomer - 20BCS3566";
  gotoxy(46, 15);cout<<"Gautam Sharma - 20BCS3548";
  gotoxy(44, 25);cout<<"Please wait while the game loads.";
  gotoxy(15, 27);
  for(int i=0; i<90; i++){printf("%c", 177);} //here the ascii symbol for loading is printed
  gotoxy(15, 27);
  for(int i=0; i<90; i++){printf("%c", 219);delay(70);} //here the ascii symbol for completed loading is printed
}

//this function displays the options in main menu
void mainmenucontent(){
  delay(100);
  gotoxy(52,14);cout<<"1. Story Mode";
  delay(100);
  gotoxy(52,16);cout<<"2. Arcade Mode";
  delay(100);
  gotoxy(52,18);cout<<"3. Pokedex";
  delay(100);
  gotoxy(52,20);cout<<"4. Exit";
  delay(100);
  gotoxy(50,26);cout<<"Enter Your Choice: ";
}

//this function displays border on the edges of application
void mainmenuborder(){
  for(int j=0;j<Vsize;j++){
    for(int i=0;i<Hsize;i++){
      if(i==0||i==1||j==0||i==Hsize-1||i==Hsize-2||j==Vsize-1){
        gotoxy(i,j);
        printf("%c", 219);
      }
    }
  }
}

//this function display a message upon selecting exit in main menu
void exitornotcontent(){
  system("cls");
  mainmenuborder();
  gotoxy(51,14);
  cout<<"Sure? Y or N : ";
}

//this function prints the "Pokemon" ASCII Art.
//intended for use only in main menu
void printtitlepokemon(){
  system("cls");
  cout<<R"(
                                                          ,'\
                            _.----.        ____         ,'  _\   ___    ___     ____
                        _,-'       `.     |    |  /`.   \,-'    |   \  /   |   |    \  |`.
                        \      __    \    '-.  | /   `.  ___    |    \/    |   '-.   \ |  |
                         \.    \ \   |  __  |  |/    ,','_  `.  |          | __  |    \|  |
                           \    \/   /,' _`.|      ,' / / / /   |          ,' _`.|     |  |
                            \     ,-'/  /   \    ,'   | \/ / ,`.|         /  /   \  |     |
                             \    \ |   \_/  |   `-.  \    `'  /|  |    ||   \_/  | |\    |
                              \    \ \      /       `-.`.___,-' |  |\  /| \      /  | |   |
                               \    \ `.__,'|  |`-._    `|      |__| \/ |  `.__,'|  | |   |
                                \_.-'       |__|    `-._ |              '-.|     '-.| |   |
                                                        `'                            '-._|
)"<<endl;
}

//this function will print the ascii art of desired pokemon
void pokemonasciiartprinter(int pokedexindexnumber){
  system("cls");
  system("Color 0B");//here the console text color is changed to light aqua
  switch(pokedexindexnumber){
  case 0://ascii art of Bulbasaur
    printf("                                           /\n");
    printf("                        _,.------....___,.' ',.-.\n");
    printf("                     ,-'          _,.--\"        |\n");
    printf("                   ,'         _.-'              .\n");
    printf("                  /   ,     ,'                   `\n");
    printf("                 .   /     /                     ``.\n");
    printf("                 |  |     .                       \\.\\\n");
    printf("       ____      |___._.  |       __               \\ `.\n");
    printf("     .'    `---\"\"       ``\"-.--\"'`  \\               .  \\\n");
    printf("    .  ,            __               `              |   .\n");
    printf("    `,'         ,-\"'  .               \\             |    L\n");
    printf("   ,'          '    _.'                -._          /    |\n");
    printf("  ,`-.    ,\".   `--'                      >.      ,'     |\n");
    printf(" . .'\\'   `-'       __    ,  ,-.         /  `.__.-      ,'\n");
    printf(" ||:, .           ,'  ;  /  / \\ `        `.    .      .'/\n");
    printf(" j|:D  \\          `--'  ' ,'_  . .         `.__, \\   , /\n");
    printf("/ L:_  |                 .  \"' :_;                `.'.'\n");
    printf(".    \"\"'                  \"\"\"\"\"'                    V\n");
    printf(" `.                                 .    `.   _,..  `\n");
    printf("   `,_   .    .                _,-'/    .. `,'   __  `\n");
    printf("    ) \\`._        ___....----\"'  ,'   .'  \\ |   '  \\  .\n");
    printf("   /   `. \"`-.--\"'         _,' ,'     `---' |    `./  |\n");
    printf("  .   _  `\"\"'--.._____..--\"   ,             '         |\n");
    printf("  | .\" `. `-.                /-.           /          ,\n");
    printf("  | `._.'    `,_            ;  /         ,'          .\n");
    printf(" .'          /| `-.        . ,'         ,           ,\n");
    printf(" '-.__ __ _,','    '`-..___;-...__   ,.'\\ ____.___.'\n");
    printf(" `\"^--'..'   '-`-^-'\"--    `-^-'`.''\"\"\"\"\"`.,^.`.--'\n");
    break;
  case 1://ascii art of Charmander
    printf("              _.--\"\"`-..\n");
    printf("            ,'          `.\n");
    printf("          ,'          __  `.\n");
    printf("         /|          \" __   \\\n");
    printf("        , |           / |.   .\n");
    printf("        |,'          !_.'|   |\n");
    printf("      ,'             '   |   |\n");
    printf("     /              |`--'|   |\n");
    printf("    |                `---'   |\n");
    printf("     .   ,                   |                       ,\".\n");
    printf("      ._     '           _'  |                    , ' \\ `\n");
    printf("  `.. `.`-...___,...---\"\"    |       __,.        ,`\"   L,|\n");
    printf("  |, `- .`._        _,-,.'   .  __.-'-. /        .   ,    \\\n");
    printf("-:..     `. `-..--_.,.<       `\"      / `.        `-/ |   .\n");
    printf("  `,         \"\"\"\"'     `.              ,'         |   |  ',,\n");
    printf("    `.      '            '            /          '    |'. |/\n");
    printf("      `.   |              \\       _,-'           |       ''\n");
    printf("        `._'               \\   '\"\\                .      |\n");
    printf("           |                '     \\                `._  ,'\n");
    printf("           |                 '     \\                 .'|\n");
    printf("           |                 .      \\                | |\n");
    printf("           |                 |       L              ,' |\n");
    printf("           `                 |       |             /   '\n");
    printf("            \\                |       |           ,'   /\n");
    printf("          ,' \\               |  _.._ ,-..___,..-'    ,'\n");
    printf("         /     .             .      `!             ,j'\n");
    printf("        /       `.          /        .           .'/\n");
    printf("       .          `.       /         |        _.'.'\n");
    printf("        `.          7`'---'          |------\"'_.'\n");
    printf("       _,.`,_     _'                ,''-----\"'\n");
    printf("   _,-_    '       `.     .'      ,\\\n");
    printf("   -\" /`.         _,'     | _  _  _.|\n");
    printf("    \"\"--'---\"\"\"\"\"'        `' '! |! /\n");
    printf("                            `\" \" -'   \n");
    printf("\n");
    printf("\n");
    break;
  case 2://ascii art of squirtle
    printf("               _,........__\n");
    printf("            ,-'            \"`-.\n");
    printf("          ,'                   `-.\n");
    printf("        ,'                        \\\n");
    printf("      ,'                           .\n");
    printf("      .'\\               ,\"\".       `\n");
    printf("     ._.'|             / |  `       \\\n");
    printf("     |   |            `-.'  ||       `.\n");
    printf("     |   |            '-._,'||       | \\\n");
    printf("     .`.,'             `..,'.'       , |`-.\n");
    printf("     l                       .'`.  _/  |   `.\n");
    printf("     `-.._'-   ,          _ _'   -\" \\  .     `\n");
    printf("`.\"\"\"\"\"'-.`-...,---------','         `. `....__.\n");
    printf(".'        `\"-..___      __,'\\          \\  \\     \\\n");
    printf("\\_ .          |   `\"\"\"\"'    `.           . \\     \\\n");
    printf("  `.          |              `.          |  .     L\n");
    printf("    `.        |`--...________.'.        j   |     |\n");
    printf("      `._    .'      |          `.     .|   ,     |\n");
    printf("         `--,\\       .            `7\"\"' |  ,      |\n");
    printf("            ` `      `            /     |  |      |    _,-'\"\"\"`-.\n");
    printf("             \\ `.     .          /      |  '      |  ,'          `.\n");
    printf("              \\  v.__  .        '       .   \\    /| /              \\\n");
    printf("               \\/    `\"\"\\\"\"\"\"\"\"\"`.       \\   \\  /.''                |\n");
    printf("                `        .        `._ ___,j.  `/ .-       ,---.     |\n");
    printf("                ,`-.      \\         .\"     `.  |/        j     `    |\n");
    printf("               /    `.     \\       /         \\ /         |     /    j\n");
    printf("              |       `-.   7-.._ .          |\"          '         /\n");
    printf("              |          `./_    `|          |            .     _,'\n");
    printf("              `.           / `----|          |-............`---'\n");
    printf("                \\          \\      |          |\n");
    printf("               ,'           )     `.         |\n");
    printf("                7____,,..--'      /          |\n");
    printf("                                  `---.__,--.'\n");
    break;
  case 3://ascii art of pikachu
    printf("                                             ,-.\n");
    printf("                                          _.|  '\n");
    printf("                                        .'  | /\n");
    printf("                                      ,'    |'\n");
    printf("                                     /      /\n");
    printf("                       _..----\"\"---.'      /\n");
    printf(" _.....---------...,-\"\"                  ,'\n");
    printf(" `-._  \\                                /\n");
    printf("     `-.+_            __           ,--. .\n");
    printf("          `-.._     .:  ).        (`--\"| \\\n");
    printf("               7    | `\" |         `...'  \\\n");
    printf("               |     `--'     '+\"        ,\". ,\"\"-\n");
    printf("               |   _...        .____     | |/    '\n");
    printf("          _.   |  .    `.  '--\"   /      `./     j\n");
    printf("         \\' `-.|  '     |   `.   /        /     /\n");
    printf("         '     `-. `---\"      `-\"        /     /\n");
    printf("          \\       `.                  _,'     /\n");
    printf("           \\        `                        .\n");
    printf("            \\                                j\n");
    printf("             \\                              /\n");
    printf("              `.                           .\n");
    printf("                +                          \\\n");
    printf("                |                           L\n");
    printf("                |                           |\n");
    printf("                |  _ /,                     |\n");
    printf("                | | L)'..                   |\n");
    printf("                | .    | `                  |\n");
    printf("                '  \\'   L                   '\n");
    printf("                 \\  \\   |                  j\n");
    printf("                  `. `__'                 /\n");
    printf("                _,.--.---........__      /\n");
    printf("               ---.,'---`         |   -j\"\n");
    printf("                .-'  '....__      L    |\n");
    printf("              \"\"--..    _,-'       \\ l||\n");
    printf("                  ,-'  .....------. `||'\n");
    printf("               _,'                /\n");
    printf("             ,'                  /\n");
    printf("            '---------+-        /\n");
    printf("                     /         /\n");
    printf("                   .'         /\n");
    printf("                 .'          /\n");
    printf("               ,'           /\n");
    printf("             _'....----\"\"\"\"\"   \n");
    break;
  case 4://ascii art of pidgey
    printf("                   .,\n");
    printf("            , _.-','\n");
    printf("          \"\"|\"    `\"\"\"\".,\n");
    printf("         /'/       __.-'-\"/\n");
    printf("        ','  _,--\"\"      '-._\n");
    printf("    _...`...'.\"\"\"\"\"\".\\\"\"-----'\n");
    printf(" ,-'          `-.) /  `.  \\\n");
    printf("+---.\"-.    |     `.    .  \\\n");
    printf("     \\  `.  |       \\   |   L\n");
    printf("      `v  ,-j        , .'   |\n");
    printf("     .'\\,' /        /,'      -._\n");
    printf("    ,____.'        .            `-.\n");
    printf("        |         /                `-.\n");
    printf("       /          `.                  `-.\n");
    printf("      /             `. |                 `.                  _.\n");
    printf("     .                `|                 ,-.             _.-\" .\n");
    printf("    j                  |                 |  \\         _.'    /\n");
    printf("    .                  |               .'    \\     ,-'      /\n");
    printf("    |                  |            ,-.\\      \\  ,'      _.-\n");
    printf("    |                . '  `.       |   `      `v'  _,.-\"/\n");
    printf("    ||    \\          |  ` |(`'-`.,.j         \\ `.-'----+---.\n");
    printf("    |'|   |L    \\  | |   `|   \\'              L \\___      /\n");
    printf("    ' L   |`     L | |     `.                 | j   `\"\"\"-'\n");
    printf("       `-'||\\    | ||j       `.       ._    ` '.\n");
    printf("          `\\ '\"`^\"- '          `.       \\    |/|\n");
    printf("            `._                  `-.     \\   Y |\n");
    printf("    __,..-\"\"`..`._                  `-._  `\\ `.|\n");
    printf("   +.....>+----.' \"\"----.........,--\"\"\" `--.'.'\n");
    printf("       ,' _\\  ,..--.-\"' __>---'  |\n");
    printf("      --\"\"  \"'  _.\" }<\"\"          `---\"\"`._\n");
    printf("               /...\"  L__.+--   _,......'..'\n");
    printf("                 /.-\"\"'/   \\ ,-'\n");
    printf("                     .' ,-\"\"'\n");
    printf("                    /.-' \n");
    break;
  case 5://ascii art of Electabuzz
    printf("              '\n");
    printf("             .\n");
    printf("             | '\n");
    printf("             | |\n");
    printf("          .  ` '                              .\n");
    printf("          '`. . .          .--.              , .\n");
    printf("       ..  `.`| `....___  '   |            ,_' |\n");
    printf("      |  |   .    _,   .`-`,-\"          ,.'    '-.\n");
    printf("      `..`._,'` .'.   '  ,'            /    .....|\n");
    printf("          `.'  ','      '  `-._      ,'     `----'\n");
    printf("          ,'| '/               `.  ,'   \\ .-\"\".   \\\n");
    printf("        |' |`'/ .|               `.     .'  _,'_.\"'\n");
    printf("       /   `.' /_|  __       _...-'   .' |.' .\"    \\\n");
    printf("     ,' `-.   '\" .-\"V `-.  ,'       ,'   |    `-'\"\"'\n");
    printf("    ,--.'. `.   /      / ,' ,      /`.   |          |\n");
    printf("  .'---'  `./\"\"|'.    / /  /      '   `-.|          |\n");
    printf(" /         /   `-....','  /              `.|    | . |\n");
    printf(".|       .'       .'.'   /      _,        .'   .  | '\n");
    printf("||       |        |/    /    _,'  7    _,\" `---'--\"'        _.._\n");
    printf("|`  .   /         |    . _.-\"    /   ,\"     |             ,'    `.\n");
    printf("`.`-', /          |   '.' ,.    /  ,'     ,-'.          .'       |\n");
    printf("  `.              |   _.-' |   '  /    _,'   |         .\"\"`.   _,'\n");
    printf("                  '.-'     '  /  /   ,\"      |        ,\"'.  |.\"\n");
    printf("                   `      .  /  /  ,'        .       /    |,'\n");
    printf("                 ,\".`.      .  '_.'        _.-`.    ,\"-.  /\n");
    printf("               .'`._`.`. '  ' ,'          `_.'\"\".  /\".  |'\n");
    printf("               |.`._`\" .`-.'           .     ___|.'   |'\n");
    printf("           ,--'  `-.' `|   `\"--..__..-\"' | .\" __|   ,'\n");
    printf("         ,' )       _,'            `'\"._.'  \"'   `-'\n");
    printf("        '\",' )__.-'\"                    `.          |.\n");
    printf("          '\"\"                             `-.  .    | |\n");
    printf("                                             `| `.-'`-.\n");
    printf("                                               `.'\n");
    printf("                                                 ' \n");
    break;
  case 6://ascii art of Vulpix
    printf("               _,.+-----__,._\n");
    printf("              /  /    ,'     `.\n");
    printf("     ,+._   ./...\\_  /   ,..   \\\n");
    printf("     | `.`+'       `-' .' ,.|  |\n");
    printf("     |  |( ,    ,.`,   |  `-',,........_       __......_\n");
    printf("      \\ |..`/,-'  '\"\"\"' `\"\"'\"  _,.---\"-,  .-+-'      _.-\"\"`--._\n");
    printf("       .\"|       /\"\\`.      ,-'       / .','      ,-'          \\\n");
    printf("      .'-'      |`-'  |    `./       / / /       /   ,.-'       |\n");
    printf("     j`v+\"      `----\"       ,'    ,'./ .'      /   |        ___|\n");
    printf("     |                      |   _,','j  |      /    L   _.-\"'    `--.\n");
    printf("      \\                     `.-'  j  |  L     F      \\-'             \\\n");
    printf("       \\ .-.               ,'     |  L   .    /    ,'       __..      `\n");
    printf("        \\ `.|            _/_      '   \\  |   /   ,'       ,\"    `.     '\n");
    printf("         `.             '   `-.    `.__| |  /  ,'         |            |\n");
    printf("           `\"-,.               `----'   `-.' .'   _,.--\"\"\"'\" --.      ,'\n");
    printf("              |          ,.                `.  ,-'              `.  _'\n");
    printf("             /|         /                    \\'          __.._    \\'\n");
    printf("   _...--...' +,..-----'                      \\-----._,-'     \\    |\n");
    printf(" ,'    |     /        \\                        \\      |       j    |\n");
    printf("/| /   |    j  ,      |                         ,._   `.    -'    /\n");
    printf("\\\\'   _`.__ | |      _L      |-----\\            `. \\    `._    _,'\n");
    printf(" \"\"`\"'     \"`\"---'\"\"`._`-._,-'      `.              `.     `--'\n");
    printf("                       \"`--.......____:.         _  / \\\n");
    printf("                               `-----.. `>-.....`,-'   \\\n");
    printf("                                      `|\"    `.  ` . \\ |\n");
    printf("                                        `._`..'    `-\"'\n");
    printf("                                           \"' \n");
    break;
  case 7://ascii art of Ratata
    printf("                                      ,'\"\"`--.\n");
    printf("                                     |     __ `-.\n");
    printf("                                     |    /  `.  `.\n");
    printf("                                      \\        ,   `.\n");
    printf("                                       `.      \\_    `.\n");
    printf("                                         `.    | `.    \\\n");
    printf("                                           `--\"    `.   `\n");
    printf("                                                     `.  `\n");
    printf("                 ,.._                                  \\  `\n");
    printf("               /_,.  `.                                 \\  `\n");
    printf("              j/   .   \\                  ___            \\  \\\n");
    printf("              |    |   `____         _,--'   `.           .  L\n");
    printf("               L  /`--\"'    `'--._ ,'   ,-`'\\ |            . |\n");
    printf("                |-                /  ,''     ||            | |\n");
    printf("     -v._      /                   ,'        ||            | |\n");
    printf("       `.`-._,'               _     \\        |j    _,...   | |\n");
    printf("         `,.'             _,-. \\     |      /,---\"\"     `- | |\n");
    printf("        .'              ,\".   ||     `..___/,'            `' |\n");
    printf("        |   ,         _/`-'  /,'                            `|\n");
    printf("        |-.__.-'\"''\"\"' \"\"\"\"\"\"\"\"--`_,...-----''''--...--      `.\n");
    printf("         `.____,..              \"\"   __,..---\"\"\"              |\n");
    printf("          |       `              --\"'.                        `\n");
    printf("          |     ,' `._                \\'                       `\n");
    printf("          | |  .^.    `.             /                          `.\n");
    printf("         ,'_]__|  \\   / `.          /          /____._            `._\n");
    printf("       ,'          \\ j    '        /          /       `.             `.\n");
    printf(" ___,.' `._       __L/    |     __'          /     _, / \\             |\n");
    printf("`-._       L,.-\"\"\"  .    ,' _.-','          /-----'-./   `--.         |\n");
    printf("   '   / ,'         '..'\"_,'    /         F /  .\"'_,'        |.__     '\n");
    printf("  / ,.\\,'              \"\"      /         / (,'\\ .'        ,.-\"'  `.  j\n");
    printf("  -'   '                      /        ,'     `\"         / __/' .- ,'\n");
    printf("                           __.'\"`.    /                 `-' | _,L,'\n");
    printf("                         .',      `\"\"'                      '/,--\n");
    printf("                          / _..' _,,'\n");
    printf("                          ,' `-\"'   \n");
    break;
  case 8://ascii art of Seal
    printf("                            _,.--\"\"\"'--._\n");
    printf("                          ,\"             `.         _,.--'\"\"\"\"--.._\n");
    printf("                         /                 `.     ,\"               `.\n");
    printf("                        |  ,                 \\   '                   `.\n");
    printf("                        '.'                   \\ /                -..   .\n");
    printf("                         j                     '                    \\  |\n");
    printf("                         |                                    .._    . |\n");
    printf("                         .    .       _...         _,..._        `.  :'\n");
    printf("                          `-./      ,'    `.      /      \"`.      |  ;\n");
    printf("                             '.   ,'       |     (          \\     .-'\n");
    printf("                               `\"'         |      \\          `-..'\n");
    printf("                                          /        \\\n");
    printf("                                        ,'          \\\n");
    printf("                                     _,'             `.\n");
    printf("                                 _,-'                  \\\n");
    printf("                               ,'                       \\\n");
    printf("                              /  /\\                      \\\n");
    printf("                             /  /  \\                      \\\n");
    printf("                            /  /    \\                      l\n");
    printf("                         _,.-\"/      '--._                 |\n");
    printf("                       ,'      `.  '      `.               |\n");
    printf("                     ,' _..          _      `.             |\n");
    printf("                    ,   _  `       ,' `.      \\            |\n");
    printf("                   .  .\".`          ...        \\           '\n");
    printf("  _.--\"\"`--....--\"'|  |`' |       .(_) |        .         /\n");
    printf(",'  _              |  `../        `.__.'        |        /\n");
    printf("| ,'              ,'-\"'--._,...  ______         |    _  /\n");
    printf("|/  _.           .  .      \\_,'\"'      `-.      '     `'..__,...----._\n");
    printf("' ,'             | | \\               ,'|  \\    /                      `\n");
    printf(" `|          _,-\"'.|  \\ .---,-._    /  |   | .' __                \"\"`-.\\\n");
    printf("  `.     _.-'      |  .:   j    `-.j   |  /.'--'  `.           .       |\n");
    printf("    `'\"\"'           `-''   '     ,'|   | ,'         `.          \\      |\n");
    printf("                        `-......'--...`-'             \\          .    ,'\n");
    printf("                                                       `-._      |_,.'\n");
    printf("                                                           `\"---\"'   \n");
    break;
  case 9://ascii art for bellsprout
    printf("                _.--\"'\"\"\"--._\n");
    printf("              .\"             `.\n");
    printf("             /                 .\n");
    printf("            j                   .\n");
    printf("            |                   |\n");
    printf("            |                   |\n");
    printf("            | (')              j\n");
    printf("            `                 ,`.\n");
    printf("             \\               ,^. `.\n");
    printf("              .             /   \\  .\n");
    printf("              |            /     . |\n");
    printf("              |          ,'      | |\n");
    printf("             ,'---..___ /        | |\n");
    printf("            ' `--..___ \"`.      .  |\n");
    printf("             `\"--....___.'     /  j__.....__\n");
    printf("                              /   |         `.\n");
    printf("                             / _,------._     `.\n");
    printf("                            /,+_         `.     `.\n");
    printf("                          ,'    `-.        \\      .\n");
    printf("                         .         `.       \\      \\\n");
    printf("                         |           `.     |       \\\n");
    printf("                         |             `.   |_,..__  .\n");
    printf("                         |\\              \\  |      `.|\n");
    printf("                         | `.,--------._  \\ |        `\n");
    printf("                         |  |           `. \\|\n");
    printf("                         |  |             `.|\n");
    printf("                         |  |\n");
    printf("                         |  |\n");
    printf("                         |. '\n");
    printf("                        .' \\ `.\n");
    printf("                     _,' ,' `. ._\n");
    printf("   ---====+,______,.\"_.-'     .  `.\n");
    printf("   _,..==`'_.+'-\"\"\"\"'         / ^.\\`:._\n");
    printf(" ,=\"/    ,\"            ....==+ /  `\\  `:.\n");
    printf("'  /    / |               _+:-'    .|   ``.\n");
    printf("       '  `.           --\"\"        |'     `\\\n");
    printf("                                     ' \n");
    break;
  }
  cout<<"Type Y to go back to Pokedex (Type Anything else to go back to main menu): ";
  char asciiartprinterselector;
  cin>>asciiartprinterselector;
  if(asciiartprinterselector=='Y'||asciiartprinterselector=='y'){
    pokedexapplicationlaunch();
  }
  else{
    main();
  }
}