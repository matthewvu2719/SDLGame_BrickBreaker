#include<SDL.h>
bool quit = false;
void Destroy();
SDL_Event event;
SDL_Renderer* renderer;
SDL_Window* window;
SDL_Surface *ball;
SDL_Surface *bg;
SDL_Surface *bat;
SDL_Surface *brick;
SDL_Texture *balltexture;
SDL_Texture *bgtexture;
SDL_Texture *battexture;
SDL_Texture *bricktexture;
SDL_Rect brickrect[3][7];
SDL_Rect ballrect;
SDL_Rect bgrect;

double ballx = 200;
double bally = 200;
double ballvelx = 0.02;
double ballvely = 0.02;
double brickw =80;
double brickh = 35;
double bgw = 800;
double bgh = 600;
double bgwmin = 0;
double bghmin = 0;
double batx = bgw/2;
double baty = bgh-30;
int delete_brick_count=0;
int no_of_bricks=21;


void InitializeBrick(){
	brickrect[0][0]={50,50,brickw,brickh};
	brickrect[0][1]={150,50,brickw,brickh};
	brickrect[0][2]={250,50,brickw,brickh};
	brickrect[0][3]={350,50,brickw,brickh};
	brickrect[0][4]={450,50,brickw,brickh};
	brickrect[0][5]={550,50,brickw,brickh};		
	brickrect[0][6]={650,50,brickw,brickh};
	brickrect[1][0]={50,100,brickw,brickh};
	brickrect[1][1]={150,100,brickw,brickh};
	brickrect[1][2]={250,100,brickw,brickh};
	brickrect[1][3]={350,100,brickw,brickh};
	brickrect[1][4]={450,100,brickw,brickh};
	brickrect[1][5]={550,100,brickw,brickh};
	brickrect[1][6]={650,100,brickw,brickh};	
	brickrect[2][0]={50,150,brickw,brickh};
	brickrect[2][1]={150,150,brickw,brickh};
	brickrect[2][2]={250,150,brickw,brickh};
	brickrect[2][3]={350,150,brickw,brickh};
	brickrect[2][4]={450,150,brickw,brickh};
	brickrect[2][5]={550,150,brickw,brickh};
	brickrect[2][6]={650,150,brickw,brickh};
}

void EventHandler(){
	SDL_PollEvent(&event);
	if(event.type==SDL_QUIT){
		quit = true;
	}
	else if(event.type == SDL_KEYDOWN){
		if(event.key.keysym.sym==SDLK_LEFT && batx>10){
			batx = batx - 30;
		}
		if(event.key.keysym.sym==SDLK_RIGHT && batx<bgw-70){
			batx = batx + 30;
		}
	}
}

void GameOver(){
	SDL_Surface*go=SDL_LoadBMP("gover.bmp");
	SDL_Texture *gotexture = SDL_CreateTextureFromSurface(renderer,go);
	SDL_Rect gorect = {0,0,bgw,bgh};
	SDL_RenderCopy(renderer,gotexture,NULL,&gorect);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
	Destroy();
	SDL_Quit();
}

void moveBall(){
	ballx = ballx + ballvelx;
	bally = bally + ballvely;
}

void ballCollision(){
	if(ballx<bgwmin||ballx>bgw-30){
		ballvelx=-ballvelx;
	}
	if(bally<bghmin){
		ballvely=-ballvely;
	}
	if(bally>bgh+60){
		GameOver();
	}	
	int ballscaling = 30;
	if(bally+ballscaling>=baty &&
	 bally+ballscaling<=baty+30 &&
	 ballx+ballscaling>=batx-2 &&
	 ballx+ballscaling<=batx+62){
	 	ballvely = -ballvely;
	 }
}

bool ball_brick_collision_detect(SDL_Rect rect1, SDL_Rect rect2){
	if(rect1.x> rect2.x+rect2.w){
		return false;
	}	
	if(rect1.x+rect1.w < rect2.x){
		return false;
	}
	if(rect1.y > rect2.y+rect2.h){
		return false;
	}
	if(rect1.y+rect1.h < rect2.y){
		return false;
	}
	else{
		return true;
	}
}


void ball_brick_collision(){
	bool a;
	for(int i=0;i<3;i++){
		for(int j=0;j<7;j++){
			a=ball_brick_collision_detect(brickrect[i][j],ballrect);
			if(a==true){
				brickrect[i][j].x=3000;
				ballvely = -ballvely;
				delete_brick_count++;
			}
			a=false;
		}
	}
}


void Destroy(){
	SDL_DestroyTexture(battexture);	
	SDL_DestroyTexture(bricktexture);
	SDL_DestroyTexture(bgtexture);
	SDL_DestroyTexture(balltexture);
	SDL_FreeSurface(bat);
	SDL_FreeSurface(brick);
	SDL_FreeSurface(bg);
	SDL_FreeSurface(ball);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void win(){
	SDL_Surface *win = SDL_LoadBMP("win.bmp");
	SDL_Texture *wintexture = SDL_CreateTextureFromSurface(renderer,win);
	SDL_Rect winrect ={250,200,350,350};
	SDL_RenderCopy(renderer,wintexture,NULL,&winrect);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);
	Destroy();
	SDL_Quit();
}


int main(int argc, char** argv){
	
	SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("The Game",
		SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,0);
		renderer = SDL_CreateRenderer(window,-1,0);
		
		
		bgrect = {0,0,800,600};
		InitializeBrick();
		ball = SDL_LoadBMP("ball.bmp");
		bg = SDL_LoadBMP("bg.bmp");
		bat = SDL_LoadBMP("bat.bmp");
		brick = SDL_LoadBMP("brick.bmp");		
		balltexture = SDL_CreateTextureFromSurface(renderer,ball);	
		bgtexture = SDL_CreateTextureFromSurface(renderer,bg);
		battexture = SDL_CreateTextureFromSurface(renderer,bat);	
		bricktexture = SDL_CreateTextureFromSurface(renderer,brick);
	
		while(!quit){
			EventHandler();
			ballrect = {ballx,bally,30,30};
			SDL_Rect batrect = {batx,baty,60,30};
			moveBall();
			ballCollision();
			ball_brick_collision();
			if(delete_brick_count ==no_of_bricks){
				win();
			}
			SDL_RenderCopy(renderer,bgtexture,NULL,&bgrect);
			SDL_RenderCopy(renderer,balltexture,NULL,&ballrect);
			SDL_RenderCopy(renderer,battexture,NULL,&batrect);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][0]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][1]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][2]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][3]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][4]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][5]);						
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[0][6]);	
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][0]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][1]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][2]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][3]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][4]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][5]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[1][6]);	
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][0]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][1]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][2]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][3]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][4]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][5]);
			SDL_RenderCopy(renderer,bricktexture,NULL,&brickrect[2][6]);	
			SDL_RenderPresent(renderer);
			SDL_RenderClear(renderer);
		}
		
	Destroy();	
	SDL_Quit();
}
