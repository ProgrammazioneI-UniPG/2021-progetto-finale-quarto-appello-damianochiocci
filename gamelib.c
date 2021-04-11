#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gamelib.h"

static unsigned short int total_quest;
static int players_number;
static struct Player* players = NULL;
static int impostors_number;
static int random_impostors;
static time_t t;
static struct Room* start_room = NULL;
static int random_type_room;
static struct Room* next_room = NULL;
static struct Room* rooms_list  = NULL;
static struct Room* first_start_room = NULL;
static int astronauts_to_kill;
static int murdered_astronauts;
static int trapdoor_random_room;
static int rooms_number;
static int trapdoor_room;
static int unmasked_impostors;
static int random_turn;
static int casual_1;
static int casual_2;
static bool impostor_movement;
static unsigned short int end_quest;
static void print_name(int name);
static int return_type_Room();
static void print_players();
static void print_status(int status);
static void type_Room(int Type);
static void first_Room();
static void start_game();
static void build_Room(struct Room* Room_built, struct Room* Previous_room);
static void players_Room();
static void run_quest();
static void move_on();
static void sabotage();
static void emergency_call();
static void kill_astronaut();
static void use_trapdoor();


// Function to set game
void Set_Game() {
 srand((unsigned)time(&t));
 printf(" enter players number for the game: \n"); // I insert the number of players verifying that that is a valid number to play
 scanf("%d", &players_number);
 while (getchar()!='\n'); //I empty the buffer
 if(players_number < 4 || players_number > 10) {
  while(players_number < 4 || players_number > 10) {
   printf("players error\n");
   scanf("%d", &players_number);
  }
 }
 printf("Players number is valid\n"); // I set the impostors number of the game randomly based on the number of players
  if (players_number<6) {
    impostors_number=1;
  } else if(players_number>5 && players_number<9) {
     random_impostors=rand()%5;
     if (random_impostors>=0 && random_impostors<3) {
       impostors_number=2;
     } else {
       impostors_number=1;
    }
   } else if(players_number>=9) {
     random_impostors=rand()%5;
     if (random_impostors>=0 && random_impostors<3) {
       impostors_number=3;
     } else {
       impostors_number=2;
     }
    }
 players=(struct Player *) calloc(players_number, sizeof(struct Player)); // Array of players
 first_Room();
 print_players();
 printf("Enter quest number: \n"); // I insert number of quests verifying that is a valid number to play
 scanf ("%hu", &total_quest);
 if (total_quest < players_number || total_quest > 17) {
   while (total_quest < players_number || total_quest > 17) {
     printf ("Quest number is not suitable, please enter number again: \n");
     scanf ("%hu", &total_quest);
   }
 }
 printf ("Quests number is correct:%hu\n", total_quest);
 printf ("Enter 1 to play or 2 to set the game: \n"); // I let the user choose to start to play or to set again the game
 static int game_setting_correct;
 scanf ("%d", &game_setting_correct);
 switch (game_setting_correct) {
   case 1:
     start_game();
     break;
   case 2:
     free (players);
     players=NULL;
     free (start_room);
     start_room=NULL;
     break;
   default:
     printf ("Choice is not suitable!\n");
     break;
 }
}


//build of first room
static void first_Room() {
 start_room=(struct Room*)malloc(sizeof(struct Room));
 start_room->Forward=NULL;
 start_room->Right=NULL;
 start_room->Left=NULL;
 start_room->Previous_room=NULL;
 start_room->Next=NULL;
 start_room->type=return_type_Room();
 start_room->emergency_call=false;
 rooms_list=start_room;
 first_start_room=start_room;
}

//Function necessary to build a room
static void build_Room(struct Room* Room_built, struct Room* Previous_room) {
  Room_built->Forward=NULL;
  Room_built->Right=NULL;
  Room_built->Left=NULL;
  Room_built->Next=NULL;
  Room_built->Previous_room=Previous_room;
  Room_built->type=return_type_Room();
  Room_built->emergency_call=false;
}

// Game can start
static void start_game () {
  confirm_setting=true;
}

//I print player's status
static void print_status(int status) {
  switch(status) {
  case 0:
   printf("Astronaut\n");
   break;
  case 1:
   printf("Impostor\n");
   break;
  case 2:
   printf("Murdered\n");
   break;
  case 3:
   printf("Ejected\n");
   break;
  }
}

//I print player's name
static void print_name(int name) {
  switch(name) {
  case 0:
   printf("Red\n");
   break;
  case 1:
   printf("Blue \n");
   break;
  case 2:
   printf("Green\n");
   break;
  case 3:
   printf("Cyan\n");
   break;
  case 4:
   printf("Brown\n");
   break;
  case 5:
   printf("Black\n");
   break;
  case 6:
   printf("Dark Green\n");
   break;
  case 7:
   printf("White\n");
   break;
  case 8:
   printf("Pink\n");
   break;
  case 9:
   printf("Violet\n");
   break;
  }
}
//I print players in random order
static void print_players() {
 for (int i = 0; i < players_number; i++) {
   players[i].Position=start_room;
   if (impostors_number!=0) {
     players[i].Status=rand()%2;
     if (players[i].Status==1) {
       impostors_number--;
     }
   } else {
     players[i].Status=0;
   }
   players[i].Name=i;
 }
 for (int i = 0; i < players_number; ++i) {
   casual_1=players[i].Name;
   casual_2=rand()%players_number;
   players[i].Name=players[casual_2].Name;
   players[casual_2].Name=casual_1;
 }
 for (int i = 0; i < players_number; ++i) {
   printf("\n%d° Player \n", i+1);
   printf("\nType of room: ");
   type_Room(players[i].Position->type);
   printf("\nstatus: ");
   print_status(players[i].Status);
   printf("\nColour: ");
   print_name(players[i].Name);
   printf("\n\n");
 }
}

//I print room type
static void type_Room(int type) {
 if (type==3) {
   printf("Trap door\n");
 } else if(type==2) {
   printf("Hard quest\n");
 } else if(type==1) {
   printf("Easy quest\n");
 } else {
   printf("Empty\n");
 }
}

//I create room type randomly
static int return_type_Room() {
 random_type_room=rand()%101;
 if (random_type_room>=0 && random_type_room<=25) {
   random_type_room=3;
 } else if (random_type_room>=26 && random_type_room<=40) {
   random_type_room=2;
 } else if (random_type_room>=41 && random_type_room<=70) {
   random_type_room=1;
 } else if (random_type_room>70) {
   random_type_room=0;
 }
 return random_type_room;
}

//Function to play
void Play() {
  static int astronauts;
  static int Astronaut_choice;
  static int impostors;
  static int Impostor_choice;
  static int turns;
  static int stop_turns;
  static int Movement_choice;
  next_room= start_room;
  int turn[players_number];
  for (int i=0;i < players_number; i++) {
    if (players[i].Status==0) {
     astronauts++;
   } else if (players[i].Status==1) {
      impostors++;
    }
  }
  while (total_quest > end_quest && impostors <astronauts-murdered_astronauts && impostors > unmasked_impostors) {
    if (turns < players_number) {
      random_turn=rand()%players_number; //I print randomly for every turn the players game order
      if (turn[random_turn]!=1 && turn[random_turn]!=2 && turn[random_turn]!=3) { //control to verify if player can play or have to wait
        if (players[random_turn].Status==0) {
          printf("\nYou're Player: ");
          print_name(players[random_turn].Name); //I print player's name
          printf("\nYou're in room: ");
          next_room= players[random_turn].Position;
          type_Room(next_room->type); //I print the type of the room where player is
          players_Room();
          printf ("\nWhat do you want to do?\n1)Move on\n2)Run quest\n3)Make an emergency call\n"); //I ask the player what he want to do, based on answer there'll be the respective function
          scanf ("%d", &Astronaut_choice);
          switch (Astronaut_choice) {
            case 1:
              printf ("Where do you want to go?\n1)Go on\n2)Right\n3)Left\n4)Stay still\n");
              scanf ("%d", &Movement_choice);
              if (Movement_choice<1 || Movement_choice>4) {
                 while (Movement_choice<1 || Movement_choice>4) {
                   printf ("Where do you want to go?\n1)Go on\n2)Right\n3)Left\n4)Stay still\n");
                   scanf ("%d", &Movement_choice);
                 }
              }
              move_on(Movement_choice);
              turn[random_turn]=1;
              break;
            case 2:
              run_quest(turn, random_turn);
              break;
            case 3:
              emergency_call();
              turn[random_turn]=1;
              break;
            default:
              printf("Choice is not suitable!\n");
              turn[random_turn]=1;
              break;
          }
          turns++;
        } else if (players[random_turn].Status==1) {
          printf("\nYou're Player: ");
          print_name(players[random_turn].Name); //I print player name
          printf("\nYou're in room: ");
          next_room= players[random_turn].Position;
          type_Room(next_room->type); //I print the room type where the player is
          players_Room();
          printf ("\nWhat do you want to do?\n1)Go on\n2)Kill astronaut\n3)Use trap door\n4)Sabotate the room\n5)Make an emergency call\n");  //I ask the player what he want to do, based on answer there'll be the respective function
          scanf ("%d", &Impostor_choice);
          switch (Impostor_choice) {
            case 1:
              printf ("Where do you want to go?\n1)Go on\n2)Right\n3)Left\n4)Stay still\n");
              scanf ("%d", &Movement_choice);
              if (Movement_choice<1 || Movement_choice>4) {
                 while (Movement_choice<1 || Movement_choice>4) {
                 printf ("Where do you want to go?\n1)Go on\n2)Right\n3)Left\n4)Stay still\n");
                 scanf ("%d", &Movement_choice);
                 }
              }
              move_on(Movement_choice);
              turn[random_turn]=1;
              break;
            case 2:
              kill_astronaut();
              turn[random_turn]=1;
              break;
            case 3:
              use_trapdoor();
              turn[random_turn]=1;
              break;
            case 4:
              sabotage();
              turn[random_turn]=2;
              break;
            case 5:
              emergency_call();
              turn[random_turn]=1;
              break;
            default:
              printf("Choice is not suitable!\n");
              turn[random_turn]=1;
              break;
          }
          turns++;
        } else if (players[random_turn].Status==2) {
            printf("\nNow is playing player : ");
            print_name(players[random_turn].Name);
            printf ("\nYou've been killed!\nYou have to wait the end of the game\n");
            turn[random_turn]=1;
            turns++;
        } else if (players[random_turn].Status==3) {
            printf("\nNow is playing player : ");
            print_name(players[random_turn].Name);
            printf ("\nYou have been ejected!\nYou have to wait the end of the game\n");
            turn[random_turn]=1;
            turns++;
        }
        if (total_quest-end_quest<=0) { //all quests have been completed
          printf("All the quests have been completed, astronauts won!\n");
          end_of_game=true;
        }
      }
    } else {
      stop_turns=0;
      for (int i=0; i<players_number; i++) {
        turn[i]--; //if the player hasn't done action that foresee staying still 1 or 2 turns then he could play right after
      }
      for (int i=0; i<players_number; i++) {
        if (turn[i]>=1) {
          stop_turns++;
        }
      }
      turns=0+stop_turns;
      }
    }
  if (impostors==unmasked_impostors) { //All impostors have been unmasked
    printf("All impostors have been unmasked, astronauts won!\n");
    end_of_game=true;
  } else if (murdered_astronauts==impostors) {
    printf("All astronauts have been eliminated, impostors won!\n");
    end_of_game=true;
  }
}

//Function that controls number and status of players present in a room
static void players_Room() {
  astronauts_to_kill=0; //variable that counts astronauts useful for the function kill_astronaut
  for (int i = 0; i <players_number; i++) {
    if (i!= random_turn) {
      if (players[random_turn].Position==players[i].Position) {
        if (players[i].Status==2) {
          printf("\nThere is a murdered player in the room..");
        }
        if (players[i].Status!=3) {
          if (players[i].Status==1) {
            printf("\nIn this room there is player: ");
            print_name(players[i].Name);
          } else if (players[i].Status==0) {
            printf("\nIn this room there is player: ");
            print_name(players[i].Name);
            astronauts_to_kill++;
          }
        }
      }
    }
  }
}



//Function that let players move in the choosed direction or stay still
static void move_on(int movement) {
    switch (movement) {
      case 1:
        next_room=players[random_turn].Position;
        if (next_room->Forward!=NULL) { //I verify if room already exist
          players[random_turn].Position=next_room->Forward;
          printf ("\nYou are arrived in the room: ");
          type_Room(next_room->Forward->type);
        } else {
          next_room->Forward=(struct Room*)malloc(sizeof(struct Room)); //I create the room because no one has been there yet
          build_Room(next_room->Forward, next_room);
          printf ("\nYou arrived in a new room: ");
          type_Room(next_room->Forward->type);
          players[random_turn].Position=next_room->Forward;
          rooms_list->Next=players[random_turn].Position;
          rooms_list=players[random_turn].Position;
          }
        players_Room();
        break;
      case 2:
        next_room=players[random_turn].Position;
        if (next_room->Right!=NULL) { //I verify if room already exist
          players[random_turn].Position=next_room->Right;
          printf ("\nYou are arrived in the room: ");
          type_Room(next_room->Right->type);
        } else {
          next_room->Right=(struct Room*)malloc(sizeof(struct Room)); //I create the room because no one has been there yet
          build_Room(next_room->Right, next_room);
          printf ("\nYou arrived in a new room: ");
          type_Room(next_room->Right->type);
          players[random_turn].Position=next_room->Right;
          rooms_list->Next=players[random_turn].Position;
          rooms_list=players[random_turn].Position;
          }
        players_Room();
        break;
      case 3:
        next_room=players[random_turn].Position;
        if (next_room->Left!=NULL) { //I verify if room already exist
          players[random_turn].Position=next_room->Left;
          printf ("\nYou are arrived in the room: ");
          type_Room(next_room->Left->type);
        } else {
          next_room->Left=(struct Room*)malloc(sizeof(struct Room)); //I create the room because no one has been there yet
          build_Room(next_room->Left, next_room);
          printf ("\nYou arrived in a new room: ");
          type_Room(next_room->Left->type);
          players[random_turn].Position=next_room->Left;
          rooms_list->Next=players[random_turn].Position;
          rooms_list=players[random_turn].Position;
          }
        players_Room();
        break;
      case 4:
        next_room=players[random_turn].Position;
        printf ("\nYou stayed in the same room where you were\n");
        players_Room();
        break;
      default:
        printf("\nChoice is not suitable!\n");
        break;
    }
}

//Function used by impostors to move to a trapdoor room
static void use_trapdoor() {
  rooms_number=0;
  trapdoor_room=0;
  impostor_movement=false;
  start_room=first_start_room;
  if (players[random_turn].Position->type==3) { //I verify if the player is in a trapdoor room
    printf("You decided to use the trap door in the room\n");
    while(start_room!=NULL) {
      if(start_room->type==3 && start_room!=players[random_turn].Position) {
        trapdoor_room++; //I increase the number of rooms with trapdoor
      }
      rooms_number++; //I increase the number of rooms without trapdoor
      start_room=start_room->Next; //I scroll through the rooms
    }
    start_room=first_start_room;
    printf("There are %d total rooms\n", rooms_number);
    int count_trapdoor[rooms_number];
    for (int i=0; i<rooms_number; i++) {
      if (start_room->type==3 && start_room!=players[random_turn].Position) {
        count_trapdoor[i]=1;
      } else {
        count_trapdoor[i]=0;
      }
      start_room=start_room->Next;
    }
    start_room=first_start_room;
    if(trapdoor_room>0) {
      printf("%d have a trap door, now you will be moved in another room, in which there's a trap door too\n", trapdoor_room);
      trapdoor_random_room=rand()%rooms_number; //
      while(!impostor_movement) {
        if (count_trapdoor[trapdoor_random_room]==0) {
          trapdoor_random_room=rand()%rooms_number;
        } else {
          while (trapdoor_random_room!=0) {
            start_room=start_room->Next;
            trapdoor_random_room--;
          }
          impostor_movement=true;
        }
      }
      players[random_turn].Position=start_room; //I update the player's position
      printf("You have been brought in the room: ");
      type_Room(players[random_turn].Position->type);
      players_Room();
    }
    else { //I send player in a random room
      printf("There aren't already trapdoor-rooms, you will be moved in a random room\n");
      trapdoor_random_room=rand()%rooms_number;
      while(trapdoor_random_room!=0) {
        start_room=start_room->Next;
        trapdoor_random_room--;
      }
      players[random_turn].Position=start_room;
      printf("You have been brought in the room: ");
      type_Room(players[random_turn].Position->type);
      players_Room();
    }
  } else {
  printf("There is no trap door in the room\n");
  }
}

//Function used in casual_2 either from astronauts and impostors if there's a murdered player in the room
static void emergency_call(){
  static int call_decision;
  static int dead_astronauts;
  static int Impostor_Ejected;
  static int Astronaut_Ejected;
  static int random_ejection;
  static int minimum_players_call;
  static bool ejection_done;
  if(players[random_turn].Position->emergency_call==false) { //I control if emergency call has already been carried out
  dead_astronauts=0;
    for(int i=0; i<players_number; i++) { //I verify the presence of murdered players
      if(players[i].Status==2 && players[i].Position==players[random_turn].Position) {
      dead_astronauts++;
      }
    }
    if(dead_astronauts>0) { //if there are murdered astronauts
      printf("In the room there are murdered astroanuts, do you want to make the emergency call?\n1) Yes\n2) No\n");
      scanf("%d", &call_decision);
      switch(call_decision){
        case 1:
          Impostor_Ejected=30; //standard chance
          Astronaut_Ejected=30; //standard chance
          for (int i=0; i<players_number; i++){
            if (players[random_turn].Position==players[i].Position) {
              if (players[i].Status==0) { //chance for astronauts
                for (int j=0; j<players_number; j++) { //I control if there are other players present
                  if (i!=j) {
                    if( players[j].Position==players[random_turn].Position) {
                      if (players[j].Status==1) {
                        Astronaut_Ejected+=20;
                      } else if(players[j].Status==0) {
                        Astronaut_Ejected-=30;
                      }
                    }
                  }
                }
              } else if(players[i].Status==1) { //chances for impostors
                for (int j=0; j<players_number; j++) {
                  if (i!=j) {
                    if (players[j].Position==players[random_turn].Position) {
                      if(players[j].Status==0) {
                        Impostor_Ejected+=20;
                      } else if (players[j].Status==1) {
                        Impostor_Ejected-=30;
                      }
                    }
                  }
                }
              }
            }
          }
          minimum_players_call=0; //I verify if beyond the murdered there are other players in the same room
          ejection_done=false;
          for (int i=0; i<players_number; i++) {
            if (players[i].Position==players[random_turn].Position) {
              if (players[i].Status==0 || players[i].Status==1) {
                minimum_players_call++;
              }
            }
          }
          if(minimum_players_call>=2) {
            if (Impostor_Ejected>Astronaut_Ejected) { //casual_2 in which impostor has more chances to be ejected
              while (!ejection_done) {
                for (int i=0; i<players_number; i++) {
                  if(players[i].Position==players[random_turn].Position) {
                    if(players[i].Status==1) {
                      printf("The ejected player is: ");
                      print_name(players[i].Name);
                      printf("\nIt was an impostor\n");
                      unmasked_impostors++;
                      players[i].Status=3;
                      ejection_done=true;
                    }
                  }
                }
              }
            } else if (Astronaut_Ejected>Impostor_Ejected) { //casual_2 in which astronaut has more chances to be ejected
              while(!ejection_done) {
              for(int i=0; i<players_number; i++) {
                if(players[i].Position==players[random_turn].Position) {
                  if(players[i].Status==0) {
                    printf("The ejected player is: ");
                    print_name(players[i].Name);
                    printf("\nIt wasn't an impostor\n");
                    murdered_astronauts++;
                    players[i].Status=3;
                    ejection_done=true;
                  }
                }
              }
            }
          } else if(Astronaut_Ejected==Impostor_Ejected) { //casual_2 in which chaces are the same
            random_ejection=rand()%2; //I entrust to casual_2 the ejection of player
            switch(random_ejection) {
              case 0:
                while (!ejection_done) {
                  for (int i=0;i<players_number;i++) {
                    if (players[i].Position==players[random_turn].Position) {
                      if (players[i].Status==1) {
                        printf("The ejected player is: ");
                        print_name(players[i].Name);
                        printf("\nIt was an impostor!\n");
                        unmasked_impostors++;
                        players[i].Status=3;
                        ejection_done=true;
                      }
                    }
                  }
                }
                break;
              case 1:
                while (!ejection_done) {
                  for (int i=0;i<players_number;i++) {
                    if (players[i].Position==players[random_turn].Position) {
                      if (players[i].Status==0) {
                        printf("The ejected player is: ");
                        print_name(players[i].Name);
                        printf("\nIt wasn't an impostor!\n");
                        murdered_astronauts++;
                        players[i].Status=3;
                        ejection_done=true;
                      }
                    }
                  }
                }
                break;
            }
          }
        } else {
          printf ("There aren't enough players in the room to eject someone\n");
        }
          players[random_turn].Position->emergency_call=true; //It will no more be possible to make the emergency call in this room
          break;
        case 2:
          printf("You decided not to use the emergency call\n");
          break;
        default:
          printf("Choice is not suitable!\n");
      }
    } else {
      printf("There aren't murdered astronauts in this room\n");
    }
  } else {
    printf("Emergency call already activated in this room\n");
  }
}
//Function that lets player complete an easy quest or a complicated one according to the type of the room which will then become empty
static void run_quest(int turn_Player[], int Playing_player) {
  if (players[random_turn].Position->type==1) {
    players[random_turn].Position->type=0;
    end_quest++;
    printf("%hu quests remain to be completed\n", total_quest-end_quest);
    printf("Room is now empty\n");
    turn_Player[Playing_player]=2; //With an easy quest the player will stay still a turn
  } else if (players[random_turn].Position->type==2) {
    players[random_turn].Position->type=0;
    end_quest+=2;
    printf("%hu quests remain to be completed\n", total_quest-end_quest);
    printf("Room is now empty\n");
    turn_Player[Playing_player]=3; //With an hard quest the player will stay still 2 turns
  } else if (players[random_turn].Position->type==0 || players[random_turn].Position->type==3) {
    printf ("There aren't quests in the room where you are\n");
    turn_Player[Playing_player]=1;
  }
}

//Function that lets impostor kill an astronaut in the same room
static void kill_astronaut() {
  static bool murder_occurred;
  static int count_players;
  static int victim_choice;
  static int Astronaut_killed;
  static int chance;
  static int partial_chance;
  if (astronauts_to_kill!=0) {
    murder_occurred=false;
    count_players=0;
    while (!murder_occurred && count_players<players_number) {
      if (count_players!=random_turn) {
        if (players[random_turn].Position==players[count_players].Position && players[count_players].Status==0) {
          print_name(players[count_players].Name);
          printf("\nIf you want to kill this player press 1, then press 2 to see the next one\n");
          scanf("%d", &victim_choice);
          switch (victim_choice) {
            case 1:
              printf("You killed player: ");
              print_name(players[count_players].Name);
              Astronaut_killed=count_players;
              murdered_astronauts++;
              players[count_players].Status=2;
              murder_occurred=true;
              break;
            case 2:
              printf("You spared player: ");
              print_name(players[count_players].Name);
              break;
            default:
              printf("Choice is not suitable!\n");
              break;
          }
        }
      }
      count_players++;
    }
  } else {
    printf ("There aren't astronauts to murder\n");
  }
  if (murder_occurred) {
    partial_chance=0;
    for (int i=0; i<players_number; i++) {
      if (i!=random_turn && i!=Astronaut_killed && players[i].Status==0) {
        if (players[i].Position==players[random_turn].Position) {
          partial_chance+=50;
        } else if (players[i].Position==players[random_turn].Position->Previous_room) {
          partial_chance+=20;
        }
      }
    }
  }
  chance=rand()%101;
  if (chance>0 && chance<=partial_chance) {
    print_name(players[random_turn].Name);
    printf(" It was an impostor so it will be ejected\n");
    players[random_turn].Status=3;
    unmasked_impostors++;
  }
  else {
    printf ("You won't be ejected, you're safe\n");
  }
}
//Function sabotage of the impostor to transform the quest room into an empty room
static void sabotage() {
  if (players[random_turn].Position->type==1) {
    players[random_turn].Position->type=0; //room becomes empty
    printf("Room is now empty\n");
  } else if (players[random_turn].Position->type==2) {
    players[random_turn].Position->type=0; //room becomes empty
    printf("Room is now empty\n");
  } else {
    printf("There are no quests to sabotate in the room \n");
  }
}

//Function to end the game,I deallocate the used memory
 void End_Game() {
   if (players!=NULL) {
     free(players);
     players=NULL;
   } else if (start_room!=NULL) {
     free(start_room);
     start_room=NULL;
   } else if (next_room!=NULL) {
    free(next_room);
     next_room=NULL;
   }
 }
