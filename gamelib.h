#include <stdbool.h>
#include <stdio.h>
bool confirm_setting;
int choice;
bool end_of_game;

enum Player_name {Red, Blue, Green, Cyan, Brown, Black, Dark_Green, White, Pink, Violet};
enum Player_status {Astronaut, Impostor, Murdered, Ejected};
enum Type_Room {Empty, Easy_quest, Hard_quest, Trapdoor};

struct Player {
  struct Room *Position;
	enum Player_status Status;
	enum Player_name Name;
};

struct Room {
	struct Room *Forward;
	struct Room *Right;
	struct Room *Left;
	struct Room *Previous_room;
  struct Room *Next;
  enum Type_Room type;
  bool emergency_call;
};

void Set_Game();
void Play();
void End_Game();
