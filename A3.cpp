#include "rsdl.hpp"
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string>
#include <vector>
#include <cmath>
#include <sstream> 
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

#define WINDOW_WIDTH 1900
#define WINDOW_HEIGHT 1080
#define MAX_STRING_LENGTH 12
#define WINDOW_TITLE "Geometry war"
#define BACK_SPACE 8
#define ENTER 13
#define UP 119
#define DOWN 115
#define LASER_MODE 115
#define SHOOT_MODE 110
#define SNIPE 122
#define SNIPE_DISABLE 120
#define BOMB 98
#define ESCAPE 27
#define SET_NAME 113
#define TICK_DURATION 10
#define PARTICLE_AGE 500
#define BOMB_AGE 250
#define MENU_X_POS 200
#define FIRST_INTERFACE 1
#define SECOND_INTERFACE 2
#define SHOOT 0
#define LASER 1
#define NEW_GAME "New Game"
#define LEADER_BOARD "Leader Board"
#define EXIT "Exit"
#define EASY "Easy"
#define NORMAL "Normal"
#define HARD "Hard"
#define BACK "Back"
#define EASY_MODE 0
#define NORMAL_MODE 1
#define HARD_MODE 2
#define PI 3.14
#define LEFT 1
#define UP_SIDE 2
#define DOWN_SIDE 3
#define RIGHT 4
#define NOT_REGISTERD -1
#define SHOOT_WIDTH 19
#define SHOOT_HEIGHT 16
#define LASER_WIDTH 25
#define LASER_HEIGHT 4
#define PARTICLE_WIDTH 20
#define PARTICLE_HEIGHT 12
#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 38
#define DRONE_WIDTH 65
#define DRONE_HEIGHT 60

#define IMG_SQUARE "./square.png"
#define IMG_BACKGROUND "./background.jpg"
#define IMG_DRONE "./drone.png"
#define IMG_BOMB "./bomb.png"
#define SHEPARD "./pori.png"
#define IMG_LASER_DRONE "./laser_drone.png"
#define IMG_ENEMY "./diamond.png"
#define IMG_PARTICLE "./particle.png"
#define IMG_SHOOT "./shoot.png"
#define IMG_LASER "./laser.png"
#define FONT_DROID "./DroidSans.ttf"

const RGB STEINER2(70,100, 180);
const RGB STEINER(159,182, 205);
const RGB SHOOT_LINE(172, 129, 137);

struct Square{
  int x,y;
  int v_x,v_y;
  int width,height;
};

struct Enemy
{
  double x,y;
  double v_x,v_y;
  int width,height;
  float angle;
  bool collid;
  bool shot;
  int num_of_particles;
};

struct Drone
{
  int x,y;
  float v_x,v_y;
  int width,height;
  float angle;
  int mode;
  int X_mouse;
  int Y_mouse;
  int life;
  int energy;
  int points;
  bool snipe_permit;
  bool bomb_permit;
};

struct Shoot
{
  int x,y;
  int v_x,v_y;
  int width,height;
  float angle;
  bool collid;
};

struct Laser
{
  int x,y;
  int v_x,v_y;
  int width,height;
  float angle;
  bool collid;
};

struct Particle
{
  double x,y;
  double v_x,v_y;
  int width,height;
  double angle;
  int age;
};

struct Score_members
{
  string member;
  int score;
};

bool Compare(const Score_members& first, const Score_members& second)
{
  return (first.score > second.score);
}

//_______________________________________________________________________________________________________________________________________________

Square create_square(int x, int y, int v_x, int v_y, int width, int height) 
{
  Square square;
  square.x = x;
  square.y = y;
  square.v_x = v_x;
  square.v_y = v_y;
  square.width = width;
  square.height = height;
  return square;
}

Enemy create_enemy(double x, double y, double v_x, double v_y, int width, int height,float angle,bool shot,int num_of_particles) 
{
  Enemy enemy;
  enemy.x = x;
  enemy.y = y;
  enemy.v_x = v_x;
  enemy.v_y = v_y;
  enemy.width = width;
  enemy.height = height;
  enemy.angle=angle;
  enemy.shot=shot;
  enemy.num_of_particles=num_of_particles;
  return enemy;
}

Drone create_drone(int x, int y, float v_x, float v_y, int width, int height,float angle,int mode,int X_mouse,int Y_mouse,int life,int energy,int points,bool snipe_permit,bool bomb_permit) 
{
  Drone drone;
  drone.x = x;
  drone.y = y;
  drone.v_x = v_x;
  drone.v_y = v_y;
  drone.width = width;
  drone.height = height;
  drone.angle=angle;
  drone.mode=mode;
  drone.X_mouse=X_mouse;
  drone.Y_mouse=Y_mouse;
  drone.life=life;
  drone.energy=energy;
  drone.points=points;
  drone.snipe_permit=snipe_permit;
  drone.bomb_permit=bomb_permit;
  return drone;
  
}

Shoot create_shoot(int x, int y, int v_x, int v_y, int width, int height,float angle,bool collid) 
{
  Shoot shoot;
  shoot.x = x;
  shoot.y = y;
  shoot.v_x = v_x;
  shoot.v_y = v_y;
  shoot.width = width;
  shoot.height = height;
  shoot.angle=angle;
  shoot.collid=collid;
  return shoot;
}

Laser create_laser(int x, int y, int v_x, int v_y, int width, int height,float angle,bool collid) 
{
  Laser laser;
  laser.x = x;
  laser.y = y;
  laser.v_x = v_x;
  laser.v_y = v_y;
  laser.width = width;
  laser.height = height;
  laser.angle=angle;
  laser.collid=collid;
  return laser;
}
Particle create_particle(double x, double y, double v_x, double v_y, int width, int height,double angle ,int age) 
{
  Particle particle;
  particle.x = x;
  particle.y = y;
  particle.v_x = v_x;
  particle.v_y = v_y;
  particle.width = width;
  particle.height = height;
  particle.angle=angle;
  particle.age=age;
  return particle;
}
//_______________________________________________________________________________________________________________________________________________
void collide_with_horizontal_border(Square& square)
{
  if (square.y<= 0 || square.y > (WINDOW_HEIGHT - square.height) )
    square.v_y = -square.v_y;
}
void collide_with_vertical_border(Square& square)
{
  if (square.x <= 0 || square.x > (WINDOW_WIDTH - square.width) )  // markze image !
    square.v_x = -square.v_x;
}
void move_square(Square& square)
{
  square.x += square.v_x;       /// to tabe asli hey ba jaye mouse chek kon bad V-x inaro check kon
  square.y += square.v_y;
  collide_with_horizontal_border(square);
  collide_with_vertical_border(square);
}
////////////////////////_________________________________________________________________________________________
void check_back(window& win,bool& flag)
{
  Event event = win.pollForEvent();
  if(event.type() == KEY_PRESS) 
  {
    if(event.pressedKey() == ENTER)
      flag=false; 
  }
}
void sort_scores(vector<Score_members>& leader_board)
{
  sort(leader_board.begin(),leader_board.end(), Compare );
}
void leader_board_back_ground(window& win,vector<Score_members>& leader_board) //adad pass bede
{
  win.clear();
  win.draw_bg(IMG_BACKGROUND, 0, 0);
  win.fill_rect(600, 10, 800, 40, STEINER);
  win.show_text("HALL OF FAMES",900,20,WHITE,FONT_DROID,24);
  win.fill_rect(900, 900, 200, 70, RED);
  win.show_text(BACK,970,920,WHITE,FONT_DROID,24);  
  if(leader_board.size())
  {
    win.show_text("Name", 200, 70, WHITE, FONT_DROID, 30);
    win.show_text("Score", 500 , 70, WHITE, FONT_DROID, 30);
    win.draw_line(420, 50, 420, 400+leader_board.size()*90, STEINER);
    win.draw_line(100, 120, 700, 120, STEINER);
  }
}
void init_scores(vector<Score_members>& leader_board,vector<string>& all_scores)
{
  stringstream string_score;
  for(int i=0;i<leader_board.size();i++)
  {
    string_score.clear();
    string_score.str("");
    string_score << leader_board[i].score;
    string score_show = string_score.str();
    all_scores.push_back(score_show);
  }
}
void leader_board_init(window& win,vector<Score_members>& leader_board)
{
  sort_scores( leader_board);
  vector<string> all_scores;
  init_scores(leader_board,all_scores);
  bool exit_to_menu=true;
  while(exit_to_menu)
  {
    leader_board_back_ground(win,leader_board);
    if(leader_board.size()==0)
    {
      win.show_text("No entry given",900,400,WHITE,FONT_DROID,30);
      win.update_screen();
      Delay(TICK_DURATION);
      check_back(win,exit_to_menu);
    }
    else
    {
      int num_of_rankers;
      if(leader_board.size()<5)
        num_of_rankers=leader_board.size();
      else
        num_of_rankers=5;//////////////////////////////////////////////////////////////////////////////////// bishtar ha mishe gozasht ----> bonus
      for(int i=0;i<num_of_rankers;i++)
      { 
        win.show_text(leader_board[i].member, 200, 140+(i*80), WHITE, FONT_DROID, 30);
        win.show_text(all_scores[i] , 500 , 140+(i*80), WHITE, FONT_DROID, 30);
        win.draw_line(100, 190+(i*80), 700, 190+(i*80), STEINER);
      }
      win.update_screen();
      Delay(TICK_DURATION);
      check_back(win,exit_to_menu);
    }
  }
}
void display_game_stats(window& win, Drone drone,bool end_show)
{
  stringstream dastor;
  win.fill_rect(20, 10, 200, 30, STEINER);
  dastor << drone.points;
  string score = dastor.str();
  win.show_text("   score  :  " + score, 50, 16, STEINER2, FONT_DROID, 20);
  if(end_show==false)
  {
    stringstream dastor2;
    dastor2 << drone.life;
    string life = dastor2.str();
    win.fill_rect(600, 10, 200, 30, STEINER);
    win.show_text("   Life  :  " + life, 650, 16, STEINER2, FONT_DROID, 20);
    stringstream dastor1;
    dastor1 << drone.energy;
    string energy = dastor1.str();
    win.fill_rect(300, 10, 200, 30, STEINER);
    win.show_text("Energy   : " +energy , 350, 16, STEINER2, FONT_DROID, 20);  
 }
}

///////////////////////_____________________________________________________________________
void draw_string(window& win, string text)
{
  win.show_text(text, 100, 100, WHITE, FONT_DROID, 30);
}
void erase_last_char(string& member_name)
{
  if(member_name.size() > 0)
      member_name.erase(member_name.size() - 1);
}
void add_char(string& member_name, char c)
{
  if(member_name.size() < MAX_STRING_LENGTH)
    member_name += c;
}
void get_name(window& win,Drone& drone,string& member_name,bool& quit_flag)
{
  Event event = win.pollForEvent();
  if(event.type() == KEY_PRESS) 
  {
    if(event.pressedKey() == BACK_SPACE)
      erase_last_char(member_name);
    else 
    {
      if(event.pressedKey() == ENTER)
        quit_flag = true;
      else
        add_char(member_name, event.pressedKey());
    }
  }
  win.clear();
  win.draw_bg(IMG_BACKGROUND, 0, 0);
  win.fill_rect(100, 100, (450), (50 - 1), RED);
  draw_string(win,"  Your name: " + member_name /*prepare_output_text(member_name)*/);
  display_game_stats(win,drone,true);
  win.update_screen();
  Delay(TICK_DURATION);
}

void check_iteration(vector<Score_members>& leader_board,Score_members& new_member,string member_name,int points)
{
  int location=NOT_REGISTERD;
  for(int i=0;i<leader_board.size();i++)
    if (leader_board[i].member==member_name)
      location=i;
  if(location==NOT_REGISTERD)
  {
    new_member.member=member_name;
    leader_board.push_back(new_member);
  }
  else if(leader_board[location].score < points)
      leader_board[location].score=points;
}

void set_record(window& win,Drone& drone,vector<Score_members>& leader_board,bool& data_added)
{
  Score_members new_member;
  bool quit_flag=false;
  new_member.score=drone.points;
  string member_name;
  while(!quit_flag)
  {
    get_name(win,drone,member_name,quit_flag);
  }
  if(member_name.size()==0)
    member_name="MR.X";
  check_iteration(leader_board,new_member,member_name,drone.points);  
  data_added=true;
}
////////////////////______________________________________________________________________________________________________________________________________________________________________graphics

void draw_in_game(window& win,Drone& drone,vector<Shoot>& shoots ,vector<Laser>& lasers,bool shoot_permit,vector<Enemy>& enemies,vector<Particle>& particles)
{
  win.draw_bg(IMG_BACKGROUND, 0, 0);

  if(drone.mode==SHOOT)
  {
    if(drone.snipe_permit==true)
    win.draw_line((drone.x+(drone.width/2 *cos(drone.angle* PI / 180))), (drone.y+(drone.width/2 *sin(drone.angle* PI / 180))), drone.X_mouse, drone.Y_mouse, SHOOT_LINE);
    win.draw_png(IMG_DRONE, drone.x, drone.y, drone.width, drone.height, (drone.angle * 180 / PI)+88 );//+95
  }
  else
  {
    win.draw_line((drone.x+(drone.width/2 *cos(drone.angle* PI / 180))), (drone.y+(drone.width/2 *sin(drone.angle* PI / 180))), drone.X_mouse, drone.Y_mouse, SHOOT_LINE);
    win.draw_png(IMG_LASER_DRONE, drone.x, drone.y, drone.width, drone.height, (drone.angle * 180 / PI)+88 );//+95
  }
  if(shoot_permit==true )
  {
    for(int i=0;i<shoots.size();i++)
      if(drone.mode==SHOOT && shoots[i].collid==false )
       win.draw_png(IMG_SHOOT, shoots[i].x, shoots[i].y, shoots[i].width, shoots[i].height, shoots[i].angle-90 );

    for(int i=0;i<lasers.size();i++)
      if(drone.mode==LASER && lasers[i].collid==false )
      win.draw_png(IMG_LASER, lasers[i].x, lasers[i].y, lasers[i].width, lasers[i].height, lasers[i].angle-90 );
  }
  for(int i=0;i<enemies.size();i++)
       win.draw_png(IMG_ENEMY, enemies[i].x,enemies[i].y, enemies[i].width, enemies[i].height, (enemies[i].angle * 180/PI)  );
  
  for(int i=0;i<particles.size();i++)
     win.draw_png(IMG_PARTICLE, particles[i].x,particles[i].y, particles[i].width, particles[i].height, (particles[i].angle )  );
 
  if(drone.bomb_permit==true)
    if(drone.energy>200)
      win.draw_png(IMG_BOMB, drone.x+30, drone.y+40, 70 ,80,(drone.angle * 180 / PI)+20 );
}

////////////////////______________________________________________________________________________________________________________________________________________________________________dynamics
void collid_drone(Drone& drone)
{
  if(drone.y > (WINDOW_HEIGHT - drone.height) )
    drone.y=(WINDOW_HEIGHT - drone.height);
  if (drone.x > (WINDOW_WIDTH - drone.width) )  // markze image !
    drone.x = (WINDOW_WIDTH - drone.width);
}
void collid_laser(Laser laser)
{
  if(laser.y<= 0 || laser.y > (WINDOW_HEIGHT -laser.height) )
  {
    laser.y = (WINDOW_HEIGHT - laser.height);
    laser.collid=true;
  }
  if (laser.x <= 0 || laser.x > (WINDOW_WIDTH - laser.width) )  // markze image !
  {
    laser.x = (WINDOW_WIDTH - laser.width);
    laser.collid=true;
  }
}
void collid_shoot(Shoot shoot)
{
  if(shoot.y<= 0 || shoot.y > (WINDOW_HEIGHT - shoot.height) )
  {
    shoot.y = (WINDOW_HEIGHT - shoot.height);
    shoot.collid=true;
  }
  if (shoot.x <= 0 || shoot.x > (WINDOW_WIDTH - shoot.width) )  // markze image !
  {
    shoot.x = (WINDOW_WIDTH -shoot.width);
    shoot.collid=true;
  }
}
void enemy_shot(vector<Enemy>& enemies,Drone& drone,vector<Shoot>& shoots,vector<Particle>& particles,int game_mode)
{
  int enemy_points[3]={20,40,80};
  for(int i=0;i<enemies.size();i++)
  {
    for(int j=0;j<shoots.size();j++)
    {
      if(abs(enemies[i].x+(enemies[i].width/2) -(shoots[j].x+(shoots[j].width/2)))<40 && abs(enemies[i].y+(enemies[i].height/2) -(shoots[j].y+(shoots[j].height/2)))<30)  // tir khorde -> shot
     {




        drone.energy+=1;
        drone.points+=enemy_points[game_mode];
        enemies[i].shot=true;////////// inja kamel knm???? ye tabe baraye  generate kardane particles
        shoots[j].collid=true;  // bara jologiri memory 




        shoots.erase(shoots.begin()+j);
        for(int k=0;k<enemies[i].num_of_particles;k++)
        {
          Particle new_particle=create_particle(enemies[i].x,enemies[i].y,1.2,1.2,PARTICLE_WIDTH,PARTICLE_HEIGHT,rand()%360 +1,0);
          particles.push_back(new_particle);
        }
        enemies.erase(enemies.begin()+i);
      }
    }
  }
}
void enemy_laserd(vector<Enemy>& enemies,Drone& drone,vector<Laser>& laser,vector<Particle>& particles,int game_mode)
{
  int enemy_points[3]={20,40,80};
  for(int i=0;i<enemies.size();i++)
  {
    for(int j=0;j<laser.size();j++)
    {
      if(abs(enemies[i].x+(enemies[i].width/2) -(laser[j].x+(laser[j].width/2)))<40 && abs(enemies[i].y+(enemies[i].height/2) -(laser[j].y+(laser[j].height/2)))<30)  // tir khorde -> shot
     {
        drone.energy+=1;
        drone.points+=enemy_points[game_mode];
        enemies[i].shot=true;////////// inja kamel knm???? ye tabe baraye  generate kardane particles
        laser[j].collid=true;  // bara jologiri memory , to draw hazf kn
        for(int k=0;k<enemies[i].num_of_particles;k++)
        {
          Particle new_particle=create_particle(enemies[i].x,enemies[i].y,1.25,1.25,PARTICLE_WIDTH,PARTICLE_HEIGHT,rand()%360 +1,0);
          particles.push_back(new_particle);        
        }
        enemies.erase(enemies.begin()+i);
      }
    }
  }
}
bool restart(window& win,Drone drone)
{
  Event event = win.pollForEvent();
  if(drone.life!=0)
  {
    if(event.type() ==KEY_PRESS )
      return true;
    else  if(event.type() ==LCLICK || event.type() ==RCLICK )
     return true;
    else
    return false;
  }
  else
    if(event.type() ==KEY_PRESS)
      if(event.pressedKey()==SET_NAME)
        return true;
    else
      return false;
}
void factory_settings(Drone& drone,vector<Enemy>& enemies,vector<Particle>& particles,vector<Laser>& laser,vector<Shoot>& shoots)
{
  drone.life=drone.life-1;
  enemies.clear();
  particles.clear();
  laser.clear();
  shoots.clear();
  drone.energy=0;
  drone.mode=SHOOT;
}
void enemy_drone_collid(window& win, Drone& drone,vector<Enemy>& enemies,vector<Particle>& particles,vector<Laser>& laser,vector<Shoot>& shoots,int game_mode)
{
  for(int i=0;i<enemies.size();i++)
  {
    bool game_start=false;
    if(abs(enemies[i].x+(enemies[i].width/2) -(drone.x+(drone.width/2)))<50 && abs(enemies[i].y+(enemies[i].height/2) -(drone.y+(drone.height/2)))<50)  // tir khorde -> shot
    {
      factory_settings( drone,enemies,particles, laser, shoots);
      win.clear();
      while(true)
      {
        win.clear();
        win.draw_bg(IMG_BACKGROUND, 0, 0);
        if(drone.life!=0)
          win.show_text("Press any thing to continue   ", 640, 500, WHITE, FONT_DROID, 40); ////////////////////////////drone ro donbale mouse bebaram???
        else
          win.show_text("Press Q  to set record   ", 640, 500, WHITE, FONT_DROID, 40); ////////////////////////////drone ro donbale mouse bebaram???        win.update_screen();
        win.update_screen();
        game_start=restart(win,drone);
        if(game_start==true)
          break;
      }
    }
    if (game_start==true)
      break;
  }
}
void collid_enemy(window& win,Drone& drone,vector<Enemy>& enemies,vector<Laser>& laser,vector<Shoot>& shoots,vector<Particle>& particles,int game_mode)  // particle ham bedm
{
  enemy_shot(enemies,drone,shoots,particles,game_mode);
  enemy_laserd( enemies,drone, laser, particles,game_mode);
  enemy_drone_collid(win,drone,enemies,particles,laser,shoots,game_mode);
}

////////////////////______________________________________________________________________________________________________________________________________________________________________
void move_enemy(Drone& drone,Enemy& enemy)
{
  if(enemy.x+(enemy.width/2) > drone.x+drone.width/2)
    enemy.v_x=-0.55;
  else
    enemy.v_x=0.55;
  if(enemy.y+(enemy.height/2) >drone.y+(drone.height/2))
    enemy.v_y=-0.55;
  else
   enemy.v_y=0.55;
  enemy.x+=enemy.v_x;
  enemy.y+=enemy.v_y;
}
void move_enemies(Drone& drone,vector<Enemy>& enemies)
{
  for(int i=0;i<enemies.size();i++)
  {
    float angle;
    angle=atan2(drone.y-enemies[i].y,drone.x-enemies[i].x);
    enemies[i].angle=angle;
    move_enemy(drone,enemies[i]);
  }
}
bool absorb_particle(Drone& drone,Particle& particle,int game_mode)
{
  int particle_points[3]={5,10,15};

  if(particle.x+(particle.width/2) > drone.x+drone.width/2)
    particle.v_x=-1.25;
  else
    particle.v_x=1.25;
  if(particle.y+(particle.height/2) >drone.y+(drone.height/2))
    particle.v_y=-1.25;
  else
    particle.v_y=1.25;
  particle.x += particle.v_x;       /// to tabe asli hey ba jaye mouse chek kon bad V-x inaro check kon
  particle.y += particle.v_y;

  if(abs(particle.x+(particle.width/2) -(drone.x+(drone.width/2)))<15 && abs(particle.y+(particle.height/2) -(drone.y+(drone.height/2)))<15)
  {
    drone.energy+=1;
     drone.points+=particle_points[game_mode];
    return true;
  }
  else 
    return false;
}
void move_particle(Particle& particle)
{
  particle.x += particle.v_x*sin(particle.angle);       /// to tabe asli hey ba jaye mouse chek kon bad V-x inaro check kon
  particle.y += particle.v_y*cos(particle.angle); 
  if (particle.y<= 0 || particle.y > (WINDOW_HEIGHT - particle.height -45) )
    particle.v_y = -particle.v_y;
  if (particle.x <= 0 || particle.x > (WINDOW_WIDTH - particle.width -45) )  // markze image !
    particle.v_x = -particle.v_x;
  particle.age ++;
}

void move_particles(Drone& drone,vector<Particle>& particles,int game_mode)
{
  for(int i=0;i<particles.size();i++)
  {
    bool is_absorbed=false;
    if(abs(particles[i].x+(particles[i].width/2) -(drone.x+(drone.width/2)))<90 && abs(particles[i].y+(particles[i].height/2) -(drone.y+(drone.height/2)))<90) 
    {
      is_absorbed=absorb_particle(drone,particles[i],game_mode);
      if(is_absorbed==true)
        particles.erase(particles.begin()+i);
    }
    else
    {
      move_particle(particles[i]);
      if(particles[i].age >=PARTICLE_AGE)
        particles.erase(particles.begin()+i);
    }
  }
}
void drone_move_process(Drone& drone, int& X_mouse,int& Y_mouse,float& angle)
{
  angle=atan2(Y_mouse-drone.y,X_mouse-drone.x);
  drone.angle=angle;
  if(drone.mode==SHOOT)
  {
    if(drone.x+(drone.width/2) > X_mouse)
      drone.v_x=-8;
    else
     drone.v_x=8;

    if(drone.y+(drone.height/2)>Y_mouse)
     drone.v_y=-8;
    else
      drone.v_y=8;
    if(abs(drone.x+(drone.width/2) -X_mouse)<95 && abs(drone.y+(drone.height/2) -Y_mouse)<95)
    {
      drone.v_y=0;
      drone.v_x=0;
    }
  }
}
void move_drone(Drone& drone, int& X_mouse,int& Y_mouse,float& angle) 
{ 
  drone_move_process(drone,X_mouse,Y_mouse,angle);
  float temp_angle=(drone.angle * 180 / PI)+88;
  if(drone.mode==SHOOT)
  {
    drone.x += drone.v_x;
    drone.y += drone.v_y;
    collid_drone(drone);
  }
}
void move_laser(vector<Laser>& lasers)
{
  for(int i=0;i<lasers.size();i++)
  {
    float angle=abs(lasers[i].angle);
    if(lasers[i].angle<=180 && lasers[i].angle>90)
    {
      lasers[i].x += lasers[i].v_x*abs(sin(angle*PI/180));
      lasers[i].y += lasers[i].v_y*abs(cos(angle*PI/180));
    }
    else if(lasers[i].angle<=90 && lasers[i].angle>0)
    {
      lasers[i].x += lasers[i].v_x*sin(angle*PI/180);
      lasers[i].y -= lasers[i].v_y*cos(angle*PI/180);

    }

    else if(lasers[i].angle<=0 && lasers[i].angle>-90)
    {
      lasers[i].x -= lasers[i].v_x*sin(angle*PI/180);
      lasers[i].y -= lasers[i].v_y*cos(angle*PI/180);
    }

    else if(lasers[i].angle<=270 && lasers[i].angle>180)
    {
      lasers[i].x -= lasers[i].v_x*abs(sin(angle*PI/180));
      lasers[i].y += lasers[i].v_y*abs(cos(angle*PI/180));
    } 
    collid_laser(lasers[i]);
  }
}
void move_shoot(vector<Shoot>& shoots,Drone drone)
{
  for(int i=0;i<shoots.size();i++)
  {
    float angle=abs(shoots[i].angle);
    if(shoots[i].angle<=180 && shoots[i].angle>90)
    {
      shoots[i].x += shoots[i].v_x*abs(sin(angle*PI/180));
      shoots[i].y += shoots[i].v_y*abs(cos(angle*PI/180));
    }
    else if(shoots[i].angle<=90 && shoots[i].angle>0)
    {
      shoots[i].x += shoots[i].v_x*sin(angle*PI/180);
      shoots[i].y -= shoots[i].v_y*cos(angle*PI/180);
    }
    else if(shoots[i].angle<=0 && shoots[i].angle>-90)
    {
      shoots[i].x -= shoots[i].v_x*sin(angle*PI/180);
      shoots[i].y -= shoots[i].v_y*cos(angle*PI/180);
    }
    else if(shoots[i].angle<=270 && shoots[i].angle>180)
    {
      shoots[i].x -= shoots[i].v_x*abs(sin(angle*PI/180));
      shoots[i].y += shoots[i].v_y*abs(cos(angle*PI/180));
    }
    collid_shoot(shoots[i]);
  }
}
void move_fire(vector<Laser>& lasers,vector<Shoot>& shoots,Drone drone) 
{
  if(drone.mode==SHOOT)
    move_shoot(shoots,drone);
  else
    move_laser(lasers);
}
void shoot_drone(vector<Shoot>& shoots,vector<Laser>& lasers,Drone& drone,int X_mouse,int Y_mouse)/////////////////// energy ina
{
  Shoot shoot=create_shoot((drone.x+(drone.width/2 *cos(drone.angle* PI / 180))),(drone.y+(drone.width/2 *sin(drone.angle* PI / 180))),30,30,SHOOT_WIDTH,SHOOT_HEIGHT,0,false);
  Laser laser=create_laser((drone.x+(drone.width/2 *cos(drone.angle* PI / 180))),(drone.y+(drone.width/2 *sin(drone.angle* PI / 180))),20,20,LASER_WIDTH,LASER_HEIGHT,0,false);
  float shoot_angle=atan2((Y_mouse-(drone.y+(drone.width/2 *sin(drone.angle* PI / 180)))),(X_mouse-(drone.x+(drone.width/2 *cos(drone.angle* PI / 180)))));
  shoot_angle=(shoot_angle * 180 / PI)+90;
  shoot.angle=shoot_angle;
  laser.angle=shoot_angle;
   if( drone.mode==LASER)
   {
    if(drone.energy>=5)
    {
      drone.energy+=-5;
      lasers.push_back(laser);  
    }
  }
  else
    shoots.push_back(shoot);
}
/*
void move_bomb(Drone& drone,Particle& bomb )
{
  float angle=atan2(WINDOW_HEIGHT/2 -drone.y,WINDOW_WIDTH/2-drone.x);
  bomb=create_particle(drone.x,drone.y,1.2,1.2,50,50,angle,0);
  while(bomb.age< BOMB_AGE)
    move_particle(bomb);
   
}
*/

void move_things(Drone& drone, int& X_mouse,int& Y_mouse,float& angle,vector<Laser>& lasers,vector<Shoot>& shoots,vector<Enemy>& enemies,vector<Particle>& particles,int game_mode/*,Particle& bomb*/)
{
  move_drone(drone,X_mouse,Y_mouse,angle);
  move_fire(lasers,shoots,drone);
  move_enemies(drone,enemies);
  move_particles(drone,particles,game_mode);
  //move_bomb(drone,bomb);
}
////////////////////______________________________________________________________________________________________________________________________________________________________________enemy
void handle_corner(int state,int& x,int& y,Drone drone)
{
  if(state==LEFT)
    x=drone.x+ (rand()%200 +200);
  else if(state=UP)
    y=drone.y+ (rand()%200 +200);
  else if(state=DOWN)
    y=drone.y- (rand()%100 +200);
  else if(state=RIGHT)
    x=drone.x- (rand()%100 +200);
}
vector<int> enemy_pos(Drone drone)//mishe bazgashti nevesht toye else ro
{
  int x=rand()%1900 +3;
  int y=rand()%1080 +3;
  vector<int>enemy_start_point;
  if(drone.x<150  )
    handle_corner(LEFT,x,y,drone);
  if(drone.y<150)
    handle_corner(UP_SIDE,x,y,drone);
  if(drone.y>930)
    handle_corner(DOWN_SIDE,x,y,drone);
  if(drone.x>1750)
    handle_corner(RIGHT,x,y,drone);
  enemy_start_point.push_back(x);
  enemy_start_point.push_back(y);
  return enemy_start_point;
}

void generate_enemy(vector<Enemy>& enemies,Drone drone,int game_mode)
{
  int boundary_score[3]={1250,750,500};
  int max_of_enemies[3]={10,20,40};
  vector<int> position=enemy_pos(drone);
  int inc_enemies=drone.points/boundary_score[game_mode];
  float angle=atan2(drone.y-position[1],drone.x-position[0]);
  if(enemies.size()<max_of_enemies[game_mode]+inc_enemies)
  {
    Enemy new_enemy=create_enemy(position[0],position[1],0.2,0.2,ENEMY_WIDTH,ENEMY_HEIGHT,angle,false,(rand()%5)+1);
    enemies.push_back(new_enemy);
  }
}

bool enemy_appear(int chance)
{
    if((rand()%100 )< chance)
      return true;
    else
      return false;
}
void generate_enemy_init(vector<Enemy>& enemies,Drone drone,int game_mode)
{
  bool is_generate;
  int chance;
  if (game_mode==0)
    chance=1;
  else if (game_mode==1)
    chance=3;
  else
    chance=8;
  is_generate=enemy_appear(chance);
  if (is_generate==true)
    generate_enemy(enemies, drone,game_mode);
}
////////////////////______________________________________________________________________________________________________________________________________________________________________Back end game process


bool in_game_process(window& win,int& X_mouse,int& Y_mouse,Drone& drone,bool& shoot_permit ,vector<Shoot>& shoots,vector<Laser>& lasers,bool& quit_game) /// ye laser be in passbedim
{
  Event event = win.pollForEvent();
  if(event.type()==MMOTION)
  {
    X_mouse=event.mouseX();
    Y_mouse=event.mouseY();
    drone.X_mouse=X_mouse;
    drone.Y_mouse=Y_mouse;
  }
  if(event.type() == KEY_PRESS)
  {
    if(event.pressedKey() == LASER_MODE)
      drone.mode=LASER;
    else if(event.pressedKey() == SHOOT_MODE)
      drone.mode=SHOOT;
    else if(event.pressedKey() == SNIPE)
      drone.snipe_permit=true;
    else if(event.pressedKey() == SNIPE_DISABLE)
      drone.snipe_permit=false;
    else if(event.pressedKey() == BOMB)
      drone.bomb_permit=true;
    else if(event.pressedKey() == SET_NAME)
      quit_game=true;
  }
  if(event.type() == LCLICK )
  {
    shoot_permit=true;
    shoot_drone(shoots,lasers,drone,X_mouse,Y_mouse); // noghte peykan
  }
}

void bomb_explosion(window& win,vector<Enemy>& enemies,vector<Laser>& lasers,Drone& drone,vector<Shoot>& shoots,vector<Particle>& particles,int game_mode)
{
  int enemy_points[3]={20,40,80};
  if(drone.bomb_permit==true)
  {
    if(drone.energy>200)
    {
      drone.energy-=200;
      for(int i=0;i<enemies.size();i++)
      {
        drone.energy+=1;
        drone.points+=enemy_points[game_mode];
        enemies[i].shot=true;
        for(int k=0;k<enemies[i].num_of_particles;k++)
          {
            Particle new_particle=create_particle(enemies[i].x,enemies[i].y,1.2,1.2,PARTICLE_WIDTH,PARTICLE_HEIGHT,rand()%360 +1,0);
            particles.push_back(new_particle);
          }
      }
      enemies.clear();
    }
  }
  drone.bomb_permit=false;
}

void initial_game(window& win ,Drone& drone,int game_mode,vector<Score_members>& leader_board) // ye vorodi baraye life !
{
  bool shoot_permit=false,data_added=false,bomb_permit=false,bomb_explode=false,quit_game=false;
  int X_mouse=0,Y_mouse=0;
  float angle;
  vector<Particle> particles;
  //Particle bomb;///////////////////////niazi be tarife structe jadid nabod!!!
  vector<Shoot> shoots;
  vector<Laser> lasers;
  vector<Enemy>enemies;
  int age=0;

  while(drone.life!=0 && quit_game!=true)
  {  
    in_game_process(win,X_mouse,Y_mouse,drone,shoot_permit,shoots,lasers,quit_game);
    generate_enemy_init(enemies,drone,game_mode);
    move_things(drone,X_mouse,Y_mouse,angle,lasers,shoots,enemies,particles,game_mode/*,bomb*/);
    win.clear();
    draw_in_game(win, drone,shoots,lasers,shoot_permit,enemies,particles);
    bomb_explosion(win,enemies,lasers, drone,shoots,particles, game_mode);
    display_game_stats(win,drone,false);
    win.update_screen();
    Delay(TICK_DURATION/1.2);
    collid_enemy(win,drone,enemies,lasers,shoots,particles,game_mode);
  }
  while(data_added==false)
    set_record(win,drone,leader_board,data_added);
}

void game_start_process(window& win,bool& game_start)
{
  Event event = win.pollForEvent();
  if(event.type() ==KEY_PRESS )
    game_start=true;
  if(event.type() ==LCLICK || event.type() ==RCLICK )
    game_start=true;
}

void start_new_game(window& win,int game_mode,vector<Score_members>& leader_board)
{
 string text="READY TO ROCK ???";
 string tips="tips: press any thing!";
 bool game_start=false;
 Drone drone = create_drone(65, 60, 10, -10, DRONE_WIDTH, DRONE_HEIGHT,0,SHOOT,0,0,3,0,0,false,false);
 while(!game_start)
 {
  game_start_process(win,game_start);
  win.clear();
  win.draw_bg(IMG_BACKGROUND, 0, 0);
  win.show_text(text, 650, 500, WHITE, FONT_DROID, 66);
  win.show_text(tips, 700, 600, WHITE, FONT_DROID, 44);
  display_game_stats(win, drone,false);
  win.update_screen();
  Delay(TICK_DURATION*1.1);
 }
 initial_game(win,drone,game_mode,leader_board);
}
////////////////////___________________________________________________________________________________________________________________________________
void second_menu_input_process(window& win, string text,int& second_state,int& depth,vector<Score_members>& leader_board)/////////////////////
{
  Event event = win.pollForEvent();
  int second_normalized_state=second_state%4;
  if(event.type() == KEY_PRESS) 
  {
    if(event.pressedKey() == UP) 
    {
      if(second_state==0)
        second_state=3;
      else
        second_state--;
    }
    else if(event.pressedKey() == DOWN)
        second_state++;
    else if(event.pressedKey() == ENTER)
    {
      if(second_normalized_state==3)
        depth=FIRST_INTERFACE;
      else if(second_normalized_state==0)
        start_new_game(win,EASY_MODE,leader_board);
      else if(second_normalized_state==1)
        start_new_game(win,NORMAL_MODE,leader_board);
      else if(second_normalized_state==2)
        start_new_game(win,HARD_MODE,leader_board);
    }
  }
}
void second_menu_color(window& win,int& choose_state,vector<int> second_menu_y_pos)
{  
  for(int i=0;i<second_menu_y_pos.size();i++)
    if(i==choose_state)
      win.fill_rect(MENU_X_POS, second_menu_y_pos[i], (240 - 1), (50 - 1), RED);
    else
      win.fill_rect(MENU_X_POS, second_menu_y_pos[i], (240 - 1), (50 - 1), BLUE);
}

void second_menu_navigate(window& win,int &choose_state)
{
  vector<int> second_menu_y_pos;
  second_menu_y_pos.push_back(100);
  second_menu_y_pos.push_back(200);
  second_menu_y_pos.push_back(300);
  second_menu_y_pos.push_back(400);
  second_menu_color(win,choose_state,second_menu_y_pos);
}
////////////////////___________________________________________________________________________________________________________________________________

void draw_string(window& win,int& depth,vector<string> menu_first,vector<string> menu_second)
{
  if(depth==FIRST_INTERFACE)
  {
    win.show_text(menu_first[0], 260, 100, WHITE, FONT_DROID, 20);
    win.show_text(menu_first[1], 260, 200, WHITE, FONT_DROID, 20);
    win.show_text(menu_first[2], 300, 300, WHITE, FONT_DROID, 20);
  }
  else
  {
    win.show_text(menu_second[0], 300, 100, WHITE, FONT_DROID, 20);
    win.show_text(menu_second[1], 300, 200, WHITE, FONT_DROID, 20);
    win.show_text(menu_second[2], 300, 300, WHITE, FONT_DROID, 20);
    win.show_text(menu_second[3], 300, 400, WHITE, FONT_DROID, 20); 
  }
}

void draw_string_init(window& win, string text,int& depth)
{
  vector<string> menu_first;
  vector<string> menu_second;
  menu_first.push_back(NEW_GAME);
  menu_first.push_back(LEADER_BOARD);
  menu_first.push_back(EXIT);
  menu_second.push_back(EASY);
  menu_second.push_back(NORMAL);
  menu_second.push_back(HARD);
  menu_second.push_back(BACK);
  draw_string(win,depth,menu_first,menu_second);
}

void first_menu_color(window& win,int& choose_state,vector<int> first_menu_y_pos)
{  
  for(int i=0;i<first_menu_y_pos.size();i++)
    if(i==choose_state)
      win.fill_rect(MENU_X_POS, first_menu_y_pos[i], (240 - 1), (50 - 1), RED);
    else
      win.fill_rect(MENU_X_POS, first_menu_y_pos[i], (240 - 1), (50 - 1), BLUE);
}


void first_menu_navigate(window& win,int& choose_state)
{
  vector<int> first_menu_y_pos;
  first_menu_y_pos.push_back(100);
  first_menu_y_pos.push_back(200);
  first_menu_y_pos.push_back(300);
  first_menu_color(win,choose_state,first_menu_y_pos);

}
void menu_navigation_process(window& win,int& state,int& depth)
{
  int choose_state;
  if(depth==FIRST_INTERFACE)
  {
    choose_state=state%3;
    first_menu_navigate(win ,choose_state);
  }  
  else
  {
    choose_state=state%4;
    second_menu_navigate(win ,choose_state);
  }
}

void draw_dynamic_background(window& win, Square square,int state,int& depth)
{
  win.draw_bg(IMG_BACKGROUND, 0, 0);
  win.draw_png(IMG_SQUARE, square.x, square.y, square.width, square.height, 45);
  menu_navigation_process(win,state,depth);
}

void bye_bye(window& win)
{
  win.clear();
  win.show_text("Hope u enjoyed !", 500, 200, WHITE, FONT_DROID, 30);
  win.draw_png(SHEPARD, 500, 400, 740, 600, 0);
  win.update_screen();
  Delay(TICK_DURATION*150);
}

void first_menu_input_process(window& win, bool& quit_flag, string& input_string,int& state,int& depth,vector<Score_members>& leader_board) ////////////////////////////////
{
  Event event = win.pollForEvent();
  int normalized_state=state%3;
  if(event.type() == KEY_PRESS) 
  {
    if(event.pressedKey() == UP) 
    {
      if(state==0)
        state=2;
      else
        state--;
    }
    else if(event.pressedKey() == DOWN)
        state++;
    else if(event.pressedKey() == ESCAPE)
        quit_flag = true;
    else if(event.pressedKey() == ENTER)
    {
      if(normalized_state==0)
      {
        depth=SECOND_INTERFACE;
      }
      else if (normalized_state==1)
        leader_board_init(win,leader_board);/////////////////////////////////////////////////////////////////////////////
      else
      {
        bye_bye(win);
        quit_flag = true;
      }
    }
  }
}

void run_input_capture_window(window& win, string& input_string ,int& depth,vector<Score_members>& leader_board)
{
  Square square = create_square(800, 500, 2, -2, 40, 40);  //  vx ina chiee
  bool quit_flag = false; //
  int state=0;
  int second_state=0;
  while(!quit_flag) 
  {
    if(depth==FIRST_INTERFACE)
    {
      first_menu_input_process(win, quit_flag, input_string ,state,depth,leader_board);
      move_square(square);
      win.clear();
      draw_dynamic_background(win, square,state,depth);  
    }
    else
    {
      second_menu_input_process(win, input_string ,second_state,depth,leader_board);
      move_square(square);
      win.clear();
      draw_dynamic_background(win, square,second_state,depth);
    }
    draw_string_init(win,(input_string),depth);
    win.update_screen();
    Delay(TICK_DURATION);
  }
}
///////////_______________________________________________________________________________________________________________
void read_score(vector<Score_members>& leader_board)
{
  ifstream infile("test.txt");
  while(infile)
  {
    Score_members new_table;
    string new_members;
    while(!(infile >> new_table.score)) 
    {
       infile.clear(); 
       if(infile >> new_members)
       new_table.member += (new_table.member.empty() ? "" : " ") + new_members;
       else
        return;
    }
    leader_board.push_back(new_table);
  }
}

void save_score(vector<Score_members>& leader_board)
{
  ofstream outfile("test.txt");
  if(outfile)
  {
    for(int i=0;i<leader_board.size();i++)
    {
      outfile<<leader_board[i].member<<"  ";
      outfile<<leader_board[i].score<<endl;
    }
  }
}

int main()
{
  vector<Score_members>leader_board;
  read_score(leader_board);
  window win(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
  string input_string;   
  int level=FIRST_INTERFACE;
  run_input_capture_window(win, input_string,level,leader_board);
  save_score(leader_board);
  return 0;
}