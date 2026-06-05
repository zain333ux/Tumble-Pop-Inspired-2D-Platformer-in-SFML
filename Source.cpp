#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf ;
using namespace std ;

// function to save game progress to file
void saveGameState(int level,float player_x,float player_y,int playerHealth,int playerScore,float* ghostX,float* ghostY,int* ghostDir,bool* ghostActive,const int numGhosts,float* skeletonX,float* skeletonY,int* skeletonDir,bool* skeletonActive,int* skeletonState,const int numSkeletons,int numCapturedEnemies,int selectedPlayer)
{
 ofstream saveFile("Assets/save.txt") ;
 
 if(!saveFile.is_open())
 {
  cout<<"ERROR: Could not create save file!"<<endl ;
  return ;
 }
 
 // writing level and player data
 saveFile<<"LEVEL "<<level<<endl ;
 saveFile<<"PLAYER_X "<<player_x<<endl ;
 saveFile<<"PLAYER_Y "<<player_y<<endl ;
 saveFile<<"HEALTH "<<playerHealth<<endl ;
 saveFile<<"SCORE "<<playerScore<<endl ;
 saveFile<<"CAPTURED "<<numCapturedEnemies<<endl ;
 saveFile<<"SELECTED_PLAYER "<<selectedPlayer<<endl ;
 
 // writing ghost data
 saveFile<<"GHOSTS "<<numGhosts<<endl ;
 for(int i=0;i<numGhosts;i++)
 {
  saveFile<<ghostX[i]<<" "<<ghostY[i]<<" "<<ghostDir[i]<<" "<<ghostActive[i]<<endl ;
 }
 
 // writing skeleton data
 saveFile<<"SKELETONS "<<numSkeletons<<endl ;
 for(int i=0;i<numSkeletons;i++)
 {
  saveFile<<skeletonX[i]<<" "<<skeletonY[i]<<" "<<skeletonDir[i]<<" "<<skeletonActive[i]<<" "<<skeletonState[i]<<endl ;
 }
 
 saveFile.close() ;
 cout<<"Game saved successfully!"<<endl ;
}

// function to load saved game
bool loadGameState(int& level,float& player_x,float& player_y,int& playerHealth,int& playerScore,float* ghostX,float* ghostY,int* ghostDir,bool* ghostActive,const int numGhosts,float* skeletonX,float* skeletonY,int* skeletonDir,bool* skeletonActive,int* skeletonState,const int numSkeletons,int& numCapturedEnemies,int& selectedPlayer)
{
 ifstream saveFile("Assets/save.txt") ;
 
 if(!saveFile.is_open())
 {
  cout<<"No save file found!"<<endl ;
  return false ;
 }
 
 // reading saved data from file
 string keyword ;
 
 saveFile>>keyword>>level ;
 saveFile>>keyword>>player_x ;
 saveFile>>keyword>>player_y ;
 saveFile>>keyword>>playerHealth ;
 saveFile>>keyword>>playerScore ;
 saveFile>>keyword>>numCapturedEnemies ;
 saveFile>>keyword>>selectedPlayer ;
 
 // loading ghost positions
 int ghostCount ;
 saveFile>>keyword>>ghostCount ;
 for(int i=0;i<ghostCount && i<numGhosts;i++)
 {
  int active ;
  saveFile>>ghostX[i]>>ghostY[i]>>ghostDir[i]>>active ;
  ghostActive[i]=(active==1) ;
 }
 
 // loading skeleton positions
 int skeletonCount ;
 saveFile>>keyword>>skeletonCount ;
 for(int i=0;i<skeletonCount && i<numSkeletons;i++)
 {
  int active ;
  saveFile>>skeletonX[i]>>skeletonY[i]>>skeletonDir[i]>>active>>skeletonState[i] ;
  skeletonActive[i]=(active==1) ;
 }
 
 saveFile.close() ;
 cout<<"Game loaded successfully!"<<endl ;
 return true ;
}

// checking if save file exists
bool saveFileExists()
{
 ifstream saveFile("Assets/save.txt") ;
 bool exists=saveFile.is_open() ;
 saveFile.close() ;
 return exists ;
}

// deleting save file
void deleteSaveFile()
{
 remove("Assets/save.txt") ;
 cout<<"Save file deleted"<<endl ;
}

// function to display level with platforms and slopes
void display_level(RenderWindow& window,char** lvl,Texture& bgTex,Sprite& bgSprite,Texture& blockTexture,Sprite& blockSprite,Texture& slopeRightTex,Texture& slopeLeftTex,Sprite& slopeSprite,const int height,const int width,const int cell_size)
{
 window.draw(bgSprite) ;
 
 int slopeTileWidth=64 ;
 int slopeTileHeight=64 ;
 
 // calculating scale for slope tiles
 float scaleX=(float)cell_size/(float)slopeTileWidth ;
 float scaleY=(float)cell_size/(float)slopeTileHeight ;
 
 // drawing all tiles in level
 for(int i=0;i<height;i++)
 {
  for(int j=0;j<width;j++)
  {
   if(lvl[i][j]=='#') // normal platform
   {
    blockSprite.setPosition(j*cell_size,i*cell_size) ;
    window.draw(blockSprite) ;
   }
   else if(lvl[i][j]=='>') // right slope
   {
    slopeSprite.setTexture(slopeRightTex) ;
    slopeSprite.setScale(scaleX,scaleY) ;
    slopeSprite.setPosition(j*cell_size,i*cell_size) ;
    window.draw(slopeSprite) ;
   }
   else if(lvl[i][j]=='<') // left slope
   {
    slopeSprite.setTexture(slopeLeftTex) ;
    slopeSprite.setScale(scaleX,scaleY) ;
    slopeSprite.setPosition(j*cell_size,i*cell_size) ;
    window.draw(slopeSprite) ;
   }
  }
 }
}

// creating platform layout for levels
void createPlatforms(char** lvl,const int height,const int width,int level)
{
 // clearing all cells first
 for(int i=0;i<height;i++)
 {
  for(int j=0;j<width;j++)
  {
   lvl[i][j]='.' ;
  }
 }
 
 // ground floor always present
 for(int j=0;j<width;j++)
 {
  lvl[height-1][j]='#' ;
 }
 
 if(level==1) // level 1 platforms
 {
  for(int j=2;j<=5;j++) lvl[3][j]='#' ;
  for(int j=12;j<=15;j++) lvl[3][j]='#' ;
  for(int j=0;j<=3;j++) lvl[5][j]='#' ;
  for(int j=7;j<=10;j++) lvl[5][j]='#' ;
  for(int j=14;j<=17;j++) lvl[5][j]='#' ;
  for(int j=4;j<=7;j++) lvl[7][j]='#' ;
  for(int j=10;j<=13;j++) lvl[7][j]='#' ;
  for(int j=1;j<=4;j++) lvl[9][j]='#' ;
  for(int j=8;j<=9;j++) lvl[9][j]='#' ;
  for(int j=13;j<=16;j++) lvl[9][j]='#' ;
  for(int j=5;j<=12;j++) lvl[11][j]='#' ;
 }
 else if(level==2) // level 2 with random platforms and slopes
 {
  // creating random top platforms
  int topChoice=rand()%3 ;
  if(topChoice==0)
  {
   for(int j=1;j<=4;j++) lvl[2][j]='#' ;
   for(int j=13;j<=16;j++) lvl[2][j]='#' ;
  }
  else if(topChoice==1)
  {
   for(int j=0;j<=5;j++) lvl[3][j]='#' ;
   for(int j=12;j<=17;j++) lvl[3][j]='#' ;
  }
  else
  {
   for(int j=2;j<=4;j++) lvl[2][j]='#' ;
   for(int j=8;j<=9;j++) lvl[3][j]='#' ;
   for(int j=14;j<=16;j++) lvl[2][j]='#' ;
  }
  
  // creating random middle-upper platforms
  int midUpChoice=rand()%3 ;
  if(midUpChoice==0)
  {
   for(int j=1;j<=3;j++) lvl[5][j]='#' ;
   for(int j=14;j<=16;j++) lvl[5][j]='#' ;
  }
  else if(midUpChoice==1)
  {
   for(int j=0;j<=4;j++) lvl[6][j]='#' ;
   for(int j=13;j<=17;j++) lvl[6][j]='#' ;
  }
  else
  {
   for(int j=2;j<=5;j++) lvl[5][j]='#' ;
   for(int j=12;j<=15;j++) lvl[5][j]='#' ;
  }
  
  // creating random middle platforms
  int midChoice=rand()%3 ;
  if(midChoice==0)
  {
   for(int j=1;j<=5;j++) lvl[8][j]='#' ;
   for(int j=12;j<=16;j++) lvl[8][j]='#' ;
  }
  else if(midChoice==1)
  {
   for(int j=0;j<=4;j++) lvl[9][j]='#' ;
   for(int j=7;j<=10;j++) lvl[9][j]='#' ;
   for(int j=13;j<=17;j++) lvl[9][j]='#' ;
  }
  else
  {
   for(int j=2;j<=4;j++) lvl[8][j]='#' ;
   for(int j=13;j<=15;j++) lvl[8][j]='#' ;
  }
  
  // bottom safe platforms always present
  for(int j=0;j<=3;j++) lvl[11][j]='#' ;
  for(int j=7;j<=10;j++) lvl[11][j]='#' ;
  for(int j=14;j<=17;j++) lvl[11][j]='#' ;
  
  // creating one long smooth slope
  int slopeType=rand()%2 ; // 0=descending right 1=descending left
  
  if(slopeType==0) // descending right slope
  {
   int startRow=2+(rand()%3) ;
   int startCol=5+(rand()%2) ;
   int slopeLength=6+(rand()%2) ;
   
   // drawing slope going down and right
   for(int i=0;i<slopeLength;i++)
   {
    int row=startRow+i ;
    int col=startCol+i ;
    
    if(row>=0 && row<height-1 && col>=0 && col<width)
    {
     lvl[row][col]='>' ; // right slanted tile
    }
   }
   
// adding landing platform at end
   int endRow=startRow+slopeLength ;
   int endCol=startCol+slopeLength ;
   if(endRow<height-1 && endCol<width-2)
   {
    for(int j=endCol;j<endCol+3 && j<width;j++)
    {
     lvl[endRow][j]='#' ;
    }
   }
  }
  else // descending left slope
  {
   int startRow=2+(rand()%3) ;
   int startCol=11+(rand()%2) ;
   int slopeLength=6+(rand()%2) ;
   
   // drawing slope going down and left
   for(int i=0;i<slopeLength;i++)
   {
    int row=startRow+i ;
    int col=startCol-i ;
    
    if(row>=0 && row<height-1 && col>=0 && col<width)
    {
     lvl[row][col]='<' ; // left slanted tile
    }
   }
   
   // adding landing platform at end
   int endRow=startRow+slopeLength ;
   int endCol=startCol-slopeLength ;
   if(endRow<height-1 && endCol>=2)
   {
    for(int j=endCol-2;j<=endCol && j>=0;j++)
    {
     lvl[endRow][j]='#' ;
    }
   }
  }
 }
}

// applying gravity to player
void player_gravity(char** lvl,float& velocityY,bool& onGround,const float& gravity,float& terminalVelocity,float& playerX,float& playerY,const int cellSize,const int height,const int width,float playerWidth,float playerHeight,bool isDropping,const int screenY)
{
 // applying gravity force
 velocityY+=gravity ;
 if(velocityY>terminalVelocity)
 {
  velocityY=terminalVelocity ;
 }
 
 // moving player down
 playerY+=velocityY ;
 
 // checking which cells player is above
 int leftCol=(int)(playerX/cellSize) ;
 int rightCol=(int)((playerX+playerWidth-1)/cellSize) ;
 int bottomRow=(int)((playerY+playerHeight+1)/cellSize) ;
 
 if(leftCol<0) leftCol=0 ;
 if(rightCol>=width) rightCol=width-1 ;
 if(bottomRow>=height) bottomRow=height-1 ;
 
 // checking if standing on platform
 bool standingOnPlatform=false ;
 char platformTile ;
 int platformCol ;
 
 for(platformCol=leftCol;platformCol<=rightCol;platformCol++)
 {
  platformTile=lvl[bottomRow][platformCol] ;
  if(platformTile=='#' || platformTile=='>' || platformTile=='<')
  {
   standingOnPlatform=true ;
   break ;
  }
 }
 
 // handling platform collision and slopes
 if(standingOnPlatform==true && velocityY>0 && isDropping==false)
 {
  float localX=(playerX+playerWidth/2)-platformCol*cellSize ;
  
  if(platformTile=='>') // right slanted slope
  {
   float slopeY=bottomRow*cellSize+localX ;
   playerY=slopeY-playerHeight ;
  }
  else if(platformTile=='<') // left slanted slope
  {
   float slopeY=bottomRow*cellSize+(cellSize-localX) ;
   playerY=slopeY-playerHeight ;
  }
  else // normal platform
  {
   playerY=bottomRow*cellSize-playerHeight ;
  }
  
  velocityY=0 ;
  onGround=true ;
 }
 else
 {
  onGround=false ;
 }
 
 // preventing fall below screen
 if(playerY+playerHeight>screenY)
 {
  playerY=screenY-playerHeight ;
  velocityY=0 ;
  onGround=true ;
 }
}
// updating player animation frames
void updateAnimation(Sprite& sprite,int& currentFrame,int maxFrames,int& animCounter,int frameWidth,int frameHeight,bool& isMoving,bool& facingRight,Texture& movingTexture,Texture& idleTexture,int idleWidth,int idleHeight,Texture& jumpTexture,int jumpWidth,int jumpHeight,int maxJumpFrames,bool& onGround)
{
 if(onGround==false) // player is jumping
 {
  sprite.setTexture(jumpTexture) ;
  animCounter++ ;
  
  if(maxJumpFrames>1 && animCounter>=15)
  {
   currentFrame++ ;
   if(currentFrame>=maxJumpFrames) currentFrame=maxJumpFrames-1 ;
   animCounter=0 ;
  }
  else
  {
   currentFrame=0 ;
  }
  
  IntRect jumpRect(currentFrame*jumpWidth,0,jumpWidth,jumpHeight) ;
  sprite.setTextureRect(jumpRect) ;
 }
 else if(isMoving==true) // player is moving
 {
  sprite.setTexture(movingTexture) ;
  animCounter++ ;
  
  if(animCounter>=15)
  {
   currentFrame++ ;
   if(currentFrame>=maxFrames) currentFrame=0 ;
   animCounter=0 ;
  }
  
  IntRect frameRect(currentFrame*frameWidth,0,frameWidth,frameHeight) ;
  sprite.setTextureRect(frameRect) ;
 }
 else // player is idle
 {
  sprite.setTexture(idleTexture) ;
  currentFrame=0 ;
  IntRect idleRect(0,0,idleWidth,idleHeight) ;
  sprite.setTextureRect(idleRect) ;
 }
 
 // flipping sprite based on direction
 if(facingRight==true)
 {
  sprite.setScale(-2,2) ;
 }
 else
 {
  sprite.setScale(2,2) ;
 }
}

// checking if platform exists at position
bool isPlatformAt(char** lvl,int col,int row,const int width,const int height)
{
 if(col<0 || col>=width || row<0 || row>=height) return false ;
 
 char tile=lvl[row][col] ;
 return (tile=='#' || tile=='>' || tile=='<') ;
}

// finding platform for skeleton to jump to
bool findJumpTarget(char** lvl,float x,float y,int dir,const int cellSize,const int width,const int height,int& targetRow,int& targetCol)
{
 int currentCol=(int)(x/cellSize) ;
 int currentRow=(int)((y+60)/cellSize) ;
 
 // searching for nearby platforms
 for(int rowOffset=-2;rowOffset<=2;rowOffset++)
 {
  if(rowOffset==0) continue ;
  
  int checkRow=currentRow+rowOffset ;
  if(checkRow<0 || checkRow>=height) continue ;
  
  for(int colOffset=2;colOffset<=4;colOffset++)
  {
   int checkCol=currentCol+(colOffset*dir) ;
   if(checkCol<0 || checkCol>=width) continue ;
   
   // found platform with empty space above
   if(isPlatformAt(lvl,checkCol,checkRow,width,height)==true)
   {
    if(isPlatformAt(lvl,checkCol,checkRow-1,width,height)==false)
    {
     targetRow=checkRow ;
     targetCol=checkCol ;
     return true ;
    }
   }
  }
 }
 
 return false ;
}

// updating skeleton enemy behavior and movement
void updateSkeletons(float* skeletonX,float* skeletonY,int* skeletonDir,bool* skeletonActive,int* skeletonState,int* skeletonFrame,int* skeletonAnimCounter,float* skeletonVelocityY,int* skeletonMoveCounter,char** lvl,const int cellSize,const int width,const int height,float skeletonSpeed,const int numSkeletons,float skeletonWidth,float skeletonHeight,const float gravity,const float terminalVelocity)
{
 for(int i=0;i<numSkeletons;i++)
 {
  if(skeletonActive[i]==false) continue ;
  
  int currentRow=(int)((skeletonY[i]+skeletonHeight+1)/cellSize) ;
  if(currentRow>=height) currentRow=height-1 ;
  
  // state 0 = idle
  if(skeletonState[i]==0)
  {
   skeletonAnimCounter[i]++ ;
   
   if(skeletonAnimCounter[i]>=30)
   {
    int choice=rand()%100 ;
    
    if(choice<30) // try to jump
    {
     int targetRow,targetCol ;
     if(findJumpTarget(lvl,skeletonX[i],skeletonY[i],skeletonDir[i],cellSize,width,height,targetRow,targetCol)==true)
     {
      skeletonState[i]=3 ; // jump state
      skeletonVelocityY[i]=-12 ;
      skeletonFrame[i]=0 ;
     }
     else
     {
      skeletonState[i]=2 ; // turn state
      skeletonFrame[i]=0 ;
     }
    }
    else // start running
    {
     skeletonState[i]=2 ; // turn state
     skeletonFrame[i]=0 ;
    }
    
    skeletonAnimCounter[i]=0 ;
   }
  }
  // state 1 = run
  else if(skeletonState[i]==1)
  {
   skeletonAnimCounter[i]++ ;
   
   // updating run animation
   if(skeletonAnimCounter[i]>=30)
   {
    skeletonFrame[i]++ ;
    if(skeletonFrame[i]>=3) skeletonFrame[i]=0 ;
    skeletonAnimCounter[i]=0 ;
   }
   
   skeletonMoveCounter[i]++ ;
   
   // checking if should stop running
   if(skeletonMoveCounter[i]>=60)
   {
    int randomAction=rand()%100 ;
    
    if(randomAction<40) // try to jump
    {
     int targetRow,targetCol ;
     if(findJumpTarget(lvl,skeletonX[i],skeletonY[i],skeletonDir[i],cellSize,width,height,targetRow,targetCol)==true)
     {
      skeletonState[i]=0 ; // idle state
      skeletonFrame[i]=0 ;
      skeletonAnimCounter[i]=0 ;
      skeletonMoveCounter[i]=0 ;
      continue ;
     }
    }
    
    skeletonMoveCounter[i]=0 ;
   }
   
   // checking platform ahead
   int checkCol ;
   if(skeletonDir[i]==1)
   {
    checkCol=(int)((skeletonX[i]+skeletonWidth+skeletonSpeed+2)/cellSize) ;
   }
   else
   {
    checkCol=(int)((skeletonX[i]-skeletonSpeed-2)/cellSize) ;
   }
   
   if(checkCol<0) checkCol=0 ;
   if(checkCol>=width) checkCol=width-1 ;
   
   bool platformAhead=false ;
   if(currentRow>=0 && currentRow<height)
   {
    platformAhead=isPlatformAt(lvl,checkCol,currentRow,width,height) ;
   }
   
   // no platform ahead so turn around
   if(platformAhead==false)
   {
    skeletonState[i]=2 ; // turn state
    skeletonFrame[i]=0 ;
    skeletonAnimCounter[i]=0 ;
    skeletonMoveCounter[i]=0 ;
   }
   else // keep moving
   {
    if(skeletonDir[i]==1) skeletonX[i]+=skeletonSpeed ;
    else skeletonX[i]-=skeletonSpeed ;
    
    // keeping skeleton in bounds
    if(skeletonX[i]<0)
    {
     skeletonX[i]=0 ;
     skeletonDir[i]=1 ;
    }
    if(skeletonX[i]+skeletonWidth>1136)
    {
     skeletonX[i]=1136-skeletonWidth ;
     skeletonDir[i]=-1 ;
    }
   }
  }
  // state 2 = turn
  else if(skeletonState[i]==2)
  {
   skeletonAnimCounter[i]++ ;
   
   if(skeletonAnimCounter[i]>=30)
   {
    skeletonFrame[i]++ ;
    
    if(skeletonFrame[i]>=2)
    {
     skeletonDir[i]*=-1 ; // flip direction
     skeletonState[i]=1 ; // run state
     skeletonFrame[i]=0 ;
    }
    
    skeletonAnimCounter[i]=0 ;
   }
  }
  // state 3 = jump
  else if(skeletonState[i]==3)
  {
   // applying gravity
   skeletonVelocityY[i]+=gravity ;
   if(skeletonVelocityY[i]>terminalVelocity) skeletonVelocityY[i]=terminalVelocity ;
   
   // moving skeleton
   skeletonY[i]+=skeletonVelocityY[i] ;
   
   if(skeletonDir[i]==1) skeletonX[i]+=skeletonSpeed*0.7f ;
   else skeletonX[i]-=skeletonSpeed*0.7f ;
   
   // checking for landing
   int bottomRow=(int)((skeletonY[i]+skeletonHeight)/cellSize) ;
   if(bottomRow>=height) bottomRow=height-1 ;
   
   int leftCol=(int)(skeletonX[i]/cellSize) ;
   int rightCol=(int)((skeletonX[i]+skeletonWidth-1)/cellSize) ;
   if(leftCol<0) leftCol=0 ;
   if(rightCol>=width) rightCol=width-1 ;
   
   bool landed=false ;
   if(skeletonVelocityY[i]>=0)
   {
    for(int col=leftCol;col<=rightCol;col++)
    {
     if(isPlatformAt(lvl,col,bottomRow,width,height)==true)
     {
      landed=true ;
      break ;
     }
    }
   }
   
   // skeleton landed on platform
   if(landed==true)
   {
    skeletonY[i]=bottomRow*cellSize-skeletonHeight ;
    skeletonVelocityY[i]=0 ;
    skeletonState[i]=0 ; // idle state
    skeletonFrame[i]=0 ;
    skeletonAnimCounter[i]=0 ;
   }
  }
 }
}

// checking collision between two rectangles
bool checkCollision(float x1,float y1,float w1,float h1,float x2,float y2,float w2,float h2)
{
 float shrink=10.0f ; // shrinking hitbox for better gameplay
 
 // calculating shrunken bounds
 float left1=x1+shrink ;
 float right1=x1+w1-shrink ;
 float top1=y1+shrink ;
 float bottom1=y1+h1-shrink ;
 
 float left2=x2+shrink ;
 float right2=x2+w2-shrink ;
 float top2=y2+shrink ;
 float bottom2=y2+h2-shrink ;
 
 // checking if rectangles overlap
 return (left1<right2 && right1>left2 && top1<bottom2 && bottom1>top2) ;
}

// updating ghost enemy movement
void updateGhosts(float* ghostX,float* ghostY,int* ghostDir,bool* ghostActive,bool* ghostTurning,char** lvl,const int cellSize,const int width,float ghostSpeed,const int numGhosts,float ghostWidth,float ghostHeight,const int screenX)
{
 for(int i=0;i<numGhosts;i++)
 {
  if(ghostActive[i]==false) continue ;
  
  // finding ghost bottom position
  int ghostBottomRow=(int)((ghostY[i]+ghostHeight+1)/cellSize) ;
  if(ghostBottomRow>=14) ghostBottomRow=13 ;
  
  // checking cell ahead of ghost
  int checkCol ;
  if(ghostDir[i]==1) checkCol=(int)((ghostX[i]+ghostWidth+ghostSpeed+2)/cellSize) ;
  else checkCol=(int)((ghostX[i]-ghostSpeed-2)/cellSize) ;
  
  if(checkCol<0) checkCol=0 ;
  if(checkCol>=width) checkCol=width-1 ;
  
  // checking if platform ahead
  bool platformAhead=false ;
  if(ghostBottomRow>=0 && ghostBottomRow<14 && checkCol>=0 && checkCol<width)
  {
   if(lvl[ghostBottomRow][checkCol]=='#') platformAhead=true ;
  }
  
  // no platform ahead so turn around
  if(platformAhead==false)
  {
   ghostDir[i]*=-1 ;
   ghostTurning[i]=true ;
  }
  
  // moving ghost
  if(ghostDir[i]==1) ghostX[i]+=ghostSpeed ;
  else ghostX[i]-=ghostSpeed ;
  
  // keeping ghost in screen bounds
  if(ghostX[i]<0)
  {
   ghostX[i]=0 ;
   ghostDir[i]=1 ;
  }
  if(ghostX[i]+ghostWidth>screenX)
  {
   ghostX[i]=screenX-ghostWidth ;
   ghostDir[i]=-1 ;
  }
 }
}

// spawning enemy on random platform
void spawnOnPlatform(char** lvl,int height,int width,int cellSize,float& x,float& y,int& dir,float enemyHeight)
{
 // trying to find valid spawn position
 for(int attempt=0;attempt<100;attempt++)
 {
  int col=2+(rand()%(width-4)) ;
  int row=2+(rand()%(height-4)) ;
  
  // found platform with empty space above
  if(lvl[row][col]=='#' && lvl[row-1][col]=='.')
  {
   x=col*cellSize ;
   y=row*cellSize-enemyHeight ;
   dir=(rand()%2==0)?1:-1 ;
   return ;
  }
 }
 
 // default spawn position if no valid position found
 x=(3+rand()%12)*cellSize ;
 y=(height-1)*cellSize-enemyHeight ;
 dir=1 ;
}

// highlighting menu text when selected
void highlightText(Text& text,bool selected,Color selectColor,Color normalColor)
{
 if(selected==true)
 {
  text.setFillColor(selectColor) ;
  text.setCharacterSize(50) ;
 }
 else
 {
  text.setFillColor(normalColor) ;
  text.setCharacterSize(45) ;
 }
}

// checking if enemy is in vacuum range
bool isInVacuumRange(float enemyX,float enemyY,float playerX,float playerY,float playerW,float playerH,int direction,float rangeH,float rangeV)
{
 float tolerance=50.0f ; // extra tolerance for easier capture
 
 if(direction==0) // right
 {
  return (enemyX>playerX && enemyX<playerX+rangeH && enemyY>playerY-tolerance && enemyY<playerY+playerH+tolerance) ;
 }
 else if(direction==1) // left
 {
  return (enemyX<playerX && enemyX>playerX-rangeH && enemyY>playerY-tolerance && enemyY<playerY+playerH+tolerance) ;
 }
 else if(direction==2) // up
 {
  return (enemyY<playerY && enemyY>playerY-rangeV && enemyX>playerX-tolerance && enemyX<playerX+playerW+tolerance) ;
 }
 else // down
 {
  return (enemyY>playerY && enemyY<playerY+rangeV && enemyX>playerX-tolerance && enemyX<playerX+playerW+tolerance) ;
 }
}
// loading leaderboard from file
void loadLeaderboard(char playerNames[][15],int playerScores[],int& totalEntries)
{
 totalEntries=0 ;
 ifstream file("Assets/leaderboard.txt") ;
 
 if(file.is_open()==false) return ;
 
 // reading all entries from file
 while(totalEntries<7 && file>>playerNames[totalEntries]>>playerScores[totalEntries])
 {
  totalEntries++ ;
 }
 
 file.close() ;
}

// saving leaderboard to file
void saveLeaderboard(char playerNames[][15],int playerScores[],int totalEntries)
{
 ofstream file("Assets/leaderboard.txt") ;
 
 for(int i=0;i<totalEntries;i++)
 {
  file<<playerNames[i]<<" "<<playerScores[i]<<endl ;
 }
 
 file.close() ;
}

// adding new score to leaderboard
void addScoreToLeaderboard(char playerNames[][15],int playerScores[],int& totalEntries,char* newName,int newScore)
{
 // finding where to insert new score
 int insertPosition=totalEntries ;
 for(int i=0;i<totalEntries;i++)
 {
  if(newScore>playerScores[i])
  {
   insertPosition=i ;
   break ;
  }
 }
 
 // shifting entries down
 for(int i=6;i>insertPosition;i--)
 {
  playerScores[i]=playerScores[i-1] ;
  
  for(int j=0;j<15;j++)
  {
   playerNames[i][j]=playerNames[i-1][j] ;
  }
 }
 
 // inserting new entry
 playerScores[insertPosition]=newScore ;
 for(int j=0;j<15;j++)
 {
  playerNames[insertPosition][j]=newName[j] ;
 }
 
 if(totalEntries<7) totalEntries++ ;
 
 saveLeaderboard(playerNames,playerScores,totalEntries) ;
}

// checking if score qualifies for leaderboard
bool isHighScore(int playerScores[],int totalEntries,int newScore)
{
 if(totalEntries<7) return true ;
 return newScore>playerScores[totalEntries-1] ;
}

// main function starts here
int main()
{
 // screen constants
 const int SCREEN_WIDTH=1136 ;
 const int SCREEN_HEIGHT=896 ;
 
 // creating game window
 RenderWindow window(VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT),"Tumble-POP",Style::Resize) ;
 window.setVerticalSyncEnabled(true) ;
 window.setFramerateLimit(60) ;
 
 // color definitions
 Color goldColor(255,215,0) ;
 Color whiteColor(255,255,255) ;
 Color grayColor(200,200,200) ;
 Color greenColor(100,255,150) ;
 Color yellowColor(255,220,100) ;
 
 // leaderboard data arrays
 char leaderboardNames[7][15] ;
 int leaderboardScores[7] ;
 int leaderboardCount=0 ;
 char inputName[15] ;
 int inputNameLength=0 ;
 
 // initializing leaderboard arrays
 for(int i=0;i<7;i++)
 {
  leaderboardScores[i]=0 ;
  for(int j=0;j<15;j++) leaderboardNames[i][j]='\0' ;
 }
 for(int i=0;i<15;i++) inputName[i]='\0' ;
 
 loadLeaderboard(leaderboardNames,leaderboardScores,leaderboardCount) ;
 
 // loading menu background
 Texture menuTexture ;
 Sprite menuSprite ;
 
 if(menuTexture.loadFromFile("Assets/menu.png")==false)
 {
  cout<<"ERROR: menu.png not found!"<<endl ;
 }
 menuSprite.setTexture(menuTexture) ;
 menuSprite.setPosition(0,0) ;
 
 // loading game font
 Font gameFont ;
 if(gameFont.loadFromFile("Assets/times.ttf")==false)
 {
  cout<<"Warning: times.ttf not found!"<<endl ;
 }
 
 // menu state variables
 int menuState=0 ; // 0=main menu
 bool gameStarted=false ;
 
 // main menu selection
 int menuSelection=0 ;
 bool hasSaveFile=saveFileExists() ;
 int totalMenuOptions=hasSaveFile?5:4 ;
 
 // creating title text
 Text titleText ;
 titleText.setFont(gameFont) ;
 titleText.setString("TUMBLE POP") ;
 titleText.setCharacterSize(80) ;
 titleText.setFillColor(goldColor) ;
 titleText.setPosition(SCREEN_WIDTH/2-250,80) ;
 
 // creating continue text
 Text continueText ;
 continueText.setFont(gameFont) ;
 continueText.setString("CONTINUE") ;
 continueText.setCharacterSize(45) ;
 continueText.setFillColor(whiteColor) ;
 continueText.setPosition(SCREEN_WIDTH/2-100,250) ;
 
 // creating new game text
 Text newGameText ;
 newGameText.setFont(gameFont) ;
 newGameText.setString("NEW GAME") ;
 newGameText.setCharacterSize(45) ;
 newGameText.setFillColor(whiteColor) ;
 newGameText.setPosition(SCREEN_WIDTH/2-110,hasSaveFile?340:280) ;
 
 // creating level select text
 Text levelSelectText ;
 levelSelectText.setFont(gameFont) ;
 levelSelectText.setString("LEVEL SELECT") ;
 levelSelectText.setCharacterSize(45) ;
 levelSelectText.setFillColor(whiteColor) ;
 levelSelectText.setPosition(SCREEN_WIDTH/2-140,hasSaveFile?430:370) ;
 
 // creating controls text
 Text controlsText ;
 controlsText.setFont(gameFont) ;
 controlsText.setString("CONTROLS") ;
 controlsText.setCharacterSize(45) ;
 controlsText.setFillColor(whiteColor) ;
 controlsText.setPosition(SCREEN_WIDTH/2-100,hasSaveFile?520:460) ;
 
 // creating high scores text
 Text highScoresText ;
 highScoresText.setFont(gameFont) ;
 highScoresText.setString("HIGH SCORES") ;
 highScoresText.setCharacterSize(45) ;
 highScoresText.setFillColor(whiteColor) ;
 highScoresText.setPosition(SCREEN_WIDTH/2-130,hasSaveFile?610:550) ;
 // player selection variables
 int playerChoice=0 ;
 int selectedPlayer=0 ;
 
 // creating player select title
 Text playerSelectTitle ;
 playerSelectTitle.setFont(gameFont) ;
 playerSelectTitle.setString("SELECT YOUR PLAYER") ;
 playerSelectTitle.setCharacterSize(60) ;
 playerSelectTitle.setFillColor(goldColor) ;
 playerSelectTitle.setPosition(SCREEN_WIDTH/2-300,100) ;
 
 // creating player 1 text
 Text player1Text ;
 player1Text.setFont(gameFont) ;
 player1Text.setString("PLAYER 1") ;
 player1Text.setCharacterSize(40) ;
 player1Text.setFillColor(whiteColor) ;
 player1Text.setPosition(220,350) ;
 
 // creating player 2 text
 Text player2Text ;
 player2Text.setFont(gameFont) ;
 player2Text.setString("PLAYER 2") ;
 player2Text.setCharacterSize(40) ;
 player2Text.setFillColor(whiteColor) ;
 player2Text.setPosition(670,350) ;
 
 // loading player textures
 Texture player1Texture,player2Texture ;
 Sprite player1Sprite,player2Sprite ;
 
 if(player1Texture.loadFromFile("Assets/player1.png")==true)
 {
  player1Sprite.setTexture(player1Texture) ;
  player1Sprite.setScale(4,4) ;
  player1Sprite.setPosition(220,420) ;
 }
 
 if(player2Texture.loadFromFile("Assets/player2.png")==true)
 {
  player2Sprite.setTexture(player2Texture) ;
  player2Sprite.setScale(4,4) ;
  player2Sprite.setPosition(670,420) ;
 }
 
 // creating selection boxes
 RectangleShape player1Box,player2Box ;
 
 // creating player instructions text
 Text playerInstructions ;
 playerInstructions.setFont(gameFont) ;
 playerInstructions.setString("Left/Right - Select    ENTER - Confirm    Backspace - Back") ;
 playerInstructions.setCharacterSize(25) ;
 playerInstructions.setFillColor(grayColor) ;
 playerInstructions.setPosition(150,730) ;
 
 // level selection variables
 int levelChoice=1 ;
 
 // creating level select title
 Text levelSelectTitle ;
 levelSelectTitle.setFont(gameFont) ;
 levelSelectTitle.setString("SELECT LEVEL") ;
 levelSelectTitle.setCharacterSize(65) ;
 levelSelectTitle.setFillColor(goldColor) ;
 levelSelectTitle.setPosition(SCREEN_WIDTH/2-220,100) ;
 
 // creating level 1 text
 Text level1Text ;
 level1Text.setFont(gameFont) ;
 level1Text.setString("LEVEL 1") ;
 level1Text.setCharacterSize(45) ;
 level1Text.setFillColor(whiteColor) ;
 level1Text.setPosition(250,300) ;
 
 // creating level 2 text
 Text level2Text ;
 level2Text.setFont(gameFont) ;
 level2Text.setString("LEVEL 2") ;
 level2Text.setCharacterSize(45) ;
 level2Text.setFillColor(whiteColor) ;
 level2Text.setPosition(670,300) ;
 
 // loading level preview images
 Texture level1Texture,level2Texture ;
 Sprite level1Sprite,level2Sprite ;
 
 if(level1Texture.loadFromFile("Assets/level1.png")==true)
 {
  level1Sprite.setTexture(level1Texture) ;
  level1Sprite.setPosition(210,380) ;
 }
 
 if(level2Texture.loadFromFile("Assets/level2.png")==true)
 {
  level2Sprite.setTexture(level2Texture) ;
  level2Sprite.setPosition(630,380) ;
 }
 
 // creating level selection boxes
 RectangleShape level1Box,level2Box ;
 
 // creating level instructions text
 Text levelInstructions ;
 levelInstructions.setFont(gameFont) ;
 levelInstructions.setString("Left/Right - Select    ENTER - Confirm    Backspace - Back") ;
 levelInstructions.setCharacterSize(25) ;
 levelInstructions.setFillColor(grayColor) ;
 levelInstructions.setPosition(150,700) ;
 
 // creating controls screen title
 Text controlsTitle ;
 controlsTitle.setFont(gameFont) ;
 controlsTitle.setString("=== GAME CONTROLS ===") ;
 controlsTitle.setCharacterSize(55) ;
 controlsTitle.setFillColor(goldColor) ;
 controlsTitle.setPosition(SCREEN_WIDTH/2-280,70) ;
 
 // creating controls content text
 Text controlsContent ;
 controlsContent.setFont(gameFont) ;
 controlsContent.setString(
  "MOVEMENT:\n"
  "  Left/Right Arrow - Move\n"
  "  Up Arrow - Jump\n"
  "  Down Arrow - Drop Through Platform\n\n"
  "COMBAT:\n"
  "  Space + W - Vacuum Up\n"
  "  Space + A - Vacuum Left\n"
  "  Space + S - Vacuum Down\n"
  "  Space + D - Vacuum Right\n"
  "  X - Shoot One Enemy\n"
  "  Z - Shoot All Enemies\n\n"
  "GAME:\n"
  "  P - Pause Game\n\n"
  "Press ENTER to return to menu"
 ) ;
 controlsContent.setCharacterSize(26) ;
 controlsContent.setFillColor(whiteColor) ;
 controlsContent.setPosition(180,180) ;
 
 bool loadFromSave=false ;
 Event event ;
 int playerScore=0 ; // player score variable
 // menu loop starts here
 while(window.isOpen()==true && gameStarted==false)
 {
  while(window.pollEvent(event))
  {
   if(event.type==Event::Closed)
   {
    window.close() ;
   }
   
   if(event.type==Event::KeyPressed)
   {
    // main menu navigation
    if(menuState==0)
    {
     if(event.key.code==Keyboard::Up)
     {
      menuSelection-- ;
      if(menuSelection<0) menuSelection=totalMenuOptions-1 ;
     }
     if(event.key.code==Keyboard::Down)
     {
      menuSelection++ ;
      if(menuSelection>=totalMenuOptions) menuSelection=0 ;
     }
     if(event.key.code==Keyboard::Return)
     {
      if(hasSaveFile==true)
      {
       if(menuSelection==0) // continue
       {
        loadFromSave=true ;
        gameStarted=true ;
        menuState=4 ;
       }
       else if(menuSelection==1) // new game
       {
        deleteSaveFile() ;
        menuState=1 ;
        playerChoice=0 ;
       }
       else if(menuSelection==2) // level select
       {
        menuState=2 ;
        levelChoice=1 ;
       }
       else if(menuSelection==3) // controls
       {
        menuState=3 ;
       }
       else if(menuSelection==4) // high scores
       {
        menuState=5 ;
       }
      }
      else
      {
       if(menuSelection==0) // new game
       {
        menuState=1 ;
        playerChoice=0 ;
       }
       else if(menuSelection==1) // level select
       {
        menuState=2 ;
        levelChoice=1 ;
       }
       else if(menuSelection==2) // controls
       {
        menuState=3 ;
       }
       else if(menuSelection==3) // high scores
       {
        menuState=5 ;
       }
      }
     }
    }
    // player selection navigation
    else if(menuState==1)
    {
     if(event.key.code==Keyboard::Left) playerChoice=0 ;
     if(event.key.code==Keyboard::Right) playerChoice=1 ;
     if(event.key.code==Keyboard::Return)
     {
      selectedPlayer=playerChoice ;
      menuState=2 ;
      levelChoice=1 ;
     }
     if(event.key.code==Keyboard::BackSpace) menuState=0 ;
    }
    // level selection navigation
    else if(menuState==2)
    {
     if(event.key.code==Keyboard::Left) levelChoice=1 ;
     if(event.key.code==Keyboard::Right) levelChoice=2 ;
     if(event.key.code==Keyboard::Return)
     {
      gameStarted=true ;
      menuState=4 ;
     }
     if(event.key.code==Keyboard::BackSpace) menuState=1 ;
    }
    // controls screen navigation
    else if(menuState==3)
    {
     if(event.key.code==Keyboard::Return || event.key.code==Keyboard::BackSpace)
      menuState=0 ;
    }
    // leaderboard screen navigation
    else if(menuState==5)
    {
     if(event.key.code==Keyboard::Return || event.key.code==Keyboard::BackSpace)
      menuState=0 ;
    }
    // name entry screen navigation
    else if(menuState==6)
    {
     if(event.key.code==Keyboard::Return && inputNameLength>0)
     {
      addScoreToLeaderboard(leaderboardNames,leaderboardScores,leaderboardCount,inputName,playerScore) ;
      menuState=5 ;
     }
     if(event.key.code==Keyboard::BackSpace && inputNameLength>0)
     {
      inputNameLength-- ;
      inputName[inputNameLength]='\0' ;
     }
    }
   }
   
   // text input for name entry
   if(menuState==6 && event.type==Event::TextEntered)
   {
    char c=event.text.unicode ;
    bool isLetter=(c>='A' && c<='Z') || (c>='a' && c<='z') ;
    if(isLetter==true && inputNameLength<14)
    {
     inputName[inputNameLength++]=c ;
     inputName[inputNameLength]='\0' ;
    }
   }
  }
  
  // pressing escape to close or go back
  if(Keyboard::isKeyPressed(Keyboard::Escape))
  {
   if(menuState==0) window.close() ;
   else menuState=0 ;
  }
  
  // updating menu highlights for main menu
  if(menuState==0)
  {
   if(hasSaveFile==true)
   {
    highlightText(continueText,menuSelection==0,goldColor,whiteColor) ;
    highlightText(newGameText,menuSelection==1,goldColor,whiteColor) ;
    highlightText(levelSelectText,menuSelection==2,goldColor,whiteColor) ;
    highlightText(controlsText,menuSelection==3,goldColor,whiteColor) ;
    highlightText(highScoresText,menuSelection==4,goldColor,whiteColor) ;
   }
   else
   {
    highlightText(newGameText,menuSelection==0,goldColor,whiteColor) ;
    highlightText(levelSelectText,menuSelection==1,goldColor,whiteColor) ;
    highlightText(controlsText,menuSelection==2,goldColor,whiteColor) ;
    highlightText(highScoresText,menuSelection==3,goldColor,whiteColor) ;
   }
  }
  
  // updating player selection highlights
  if(menuState==1)
  {
   highlightText(player1Text,playerChoice==0,greenColor,whiteColor) ;
   highlightText(player2Text,playerChoice==1,yellowColor,whiteColor) ;
   
   float scale1=(playerChoice==0)?4.3f:4.0f ;
   float scale2=(playerChoice==1)?4.3f:4.0f ;
   player1Sprite.setScale(scale1,scale1) ;
   player2Sprite.setScale(scale2,scale2) ;
   
   player1Box.setFillColor((playerChoice==0)?Color(100,255,150,80):Color(100,100,100,30)) ;
   player2Box.setFillColor((playerChoice==1)?Color(255,220,100,80):Color(100,100,100,30)) ;
  }
  
  // updating level selection highlights
  if(menuState==2)
  {
   highlightText(level1Text,levelChoice==1,goldColor,whiteColor) ;
   highlightText(level2Text,levelChoice==2,goldColor,whiteColor) ;
   
   float scale1=(levelChoice==1)?1.1f:1.0f ;
   float scale2=(levelChoice==2)?1.1f:1.0f ;
   level1Sprite.setScale(scale1,scale1) ;
   level2Sprite.setScale(scale2,scale2) ;
   
   level1Box.setFillColor((levelChoice==1)?Color(255,215,0,60):Color(100,100,100,20)) ;
   level2Box.setFillColor((levelChoice==2)?Color(255,215,0,60):Color(100,100,100,20)) ;
  }
  // drawing menu screens
  window.clear() ;
  window.draw(menuSprite) ;
  
  // drawing main menu
  if(menuState==0)
  {
   window.draw(titleText) ;
   if(hasSaveFile==true) window.draw(continueText) ;
   window.draw(newGameText) ;
   window.draw(levelSelectText) ;
   window.draw(controlsText) ;
   window.draw(highScoresText) ;
  }
  // drawing player selection screen
  else if(menuState==1)
  {
   window.draw(playerSelectTitle) ;
   window.draw(player1Box) ;
   window.draw(player2Box) ;
   window.draw(player1Sprite) ;
   window.draw(player2Sprite) ;
   window.draw(player1Text) ;
   window.draw(player2Text) ;
   window.draw(playerInstructions) ;
  }
  // drawing level selection screen
  else if(menuState==2)
  {
   window.draw(levelSelectTitle) ;
   window.draw(level1Box) ;
   window.draw(level2Box) ;
   window.draw(level1Sprite) ;
   window.draw(level2Sprite) ;
   window.draw(level1Text) ;
   window.draw(level2Text) ;
   window.draw(levelInstructions) ;
  }
  // drawing controls screen
  else if(menuState==3)
  {
   window.draw(controlsTitle) ;
   window.draw(controlsContent) ;
  }
  // drawing leaderboard screen
  else if(menuState==5)
  {
   Text title ;
   title.setFont(gameFont) ;
   title.setString("=== HIGH SCORES ===") ;
   title.setCharacterSize(55) ;
   title.setFillColor(goldColor) ;
   title.setPosition(SCREEN_WIDTH/2-230,80) ;
   window.draw(title) ;
   
   // drawing all leaderboard entries
   for(int i=0;i<7;i++)
   {
    char line[30] ;
    line[0]='1'+i ;
    line[1]='.' ;
    line[2]=' ' ;
    int pos=3 ;
    
    if(i<leaderboardCount)
    {
     int n=0 ;
     while(leaderboardNames[i][n]!='\0')
     {
      line[pos++]=leaderboardNames[i][n++] ;
     }
     while(pos<14) line[pos++]='.' ;
     
     // converting score to string
     int score=leaderboardScores[i] ;
     char digits[10] ;
     int digitCount=0 ;
     if(score==0) digits[digitCount++]='0' ;
     while(score>0)
     {
      digits[digitCount++]='0'+score%10 ;
      score/=10 ;
     }
     while(digitCount>0) line[pos++]=digits[--digitCount] ;
    }
    else
    {
     line[pos++]='-' ;
     line[pos++]='-' ;
     line[pos++]='-' ;
    }
    line[pos]='\0' ;
    
    Text entry ;
    entry.setFont(gameFont) ;
    entry.setString(line) ;
    entry.setCharacterSize(32) ;
    entry.setFillColor(whiteColor) ;
    entry.setPosition(320,160+i*50) ;
    window.draw(entry) ;
   }
   
   Text backText ;
   backText.setFont(gameFont) ;
   backText.setString("ENTER to return") ;
   backText.setCharacterSize(25) ;
   backText.setFillColor(grayColor) ;
   backText.setPosition(420,550) ;
   window.draw(backText) ;
  }
  // drawing name entry screen
  else if(menuState==6)
  {
   Text title ;
   title.setFont(gameFont) ;
   title.setString("NEW HIGH SCORE!") ;
   title.setCharacterSize(55) ;
   title.setFillColor(goldColor) ;
   title.setPosition(SCREEN_WIDTH/2-220,200) ;
   window.draw(title) ;
   
   Text nameText ;
   nameText.setFont(gameFont) ;
   nameText.setString(inputName) ;
   nameText.setCharacterSize(50) ;
   nameText.setFillColor(yellowColor) ;
   nameText.setPosition(SCREEN_WIDTH/2-80,320) ;
   window.draw(nameText) ;
   
   Text hintText ;
   hintText.setFont(gameFont) ;
   hintText.setString("Type name + ENTER") ;
   hintText.setCharacterSize(28) ;
   hintText.setFillColor(grayColor) ;
   hintText.setPosition(SCREEN_WIDTH/2-130,420) ;
   window.draw(hintText) ;
  }
  
  window.display() ;
 }
 
 // printing game start info
 cout<<"\n========================================"<<endl ;
 cout<<"     GAME STARTING..."<<endl ;
 cout<<"     Level: "<<levelChoice<<endl ;
 cout<<"     Player: "<<(selectedPlayer+1)<<endl ;
 cout<<"====================================\n"<<endl ;
 
 // game setup starts here
 const int CELL_SIZE=64 ;
 const int MAP_HEIGHT=14 ;
 const int MAP_WIDTH=18 ;
 char** levelMap ;
 
 // loading background textures
 Texture backgroundTexture ;
 Sprite backgroundSprite ;
 Texture blockTexture ;
 Sprite blockSprite ;
 
 backgroundTexture.loadFromFile("Assets/bg.png") ;
 backgroundSprite.setTexture(backgroundTexture) ;
 backgroundSprite.setPosition(0,0) ;
 
 blockTexture.loadFromFile("Assets/block1.png") ;
 blockSprite.setTexture(blockTexture) ;
 
 // loading slope textures
 Texture slopeRightTex,slopeLeftTex ;
 Sprite slopeSprite ;
 slopeRightTex.loadFromFile("Assets/sloperight.png") ;
 slopeLeftTex.loadFromFile("Assets/slopeleft.png") ;
 
 // loading and playing game music
 Music gameMusic ;
 gameMusic.openFromFile("Assets/mus.ogg") ;
 gameMusic.setVolume(20) ;
 gameMusic.play() ;
 gameMusic.setLoop(true) ;
 
 // loading heart texture for health display
 Texture heartTexture ;
 Sprite heartSprite ;
 heartTexture.loadFromFile("Assets/heart.png") ;
 heartSprite.setTexture(heartTexture) ;
 heartSprite.setScale(2,2) ;
 
 // player movement variables
 float moveSpeed=4.0f ;
 const float JUMP_STRENGTH=-12.3f ;
 const float GRAVITY=0.5f ;
 
 // loading player textures
 Texture playerMoveTexture,playerIdleTexture,playerJumpTexture ;
 Sprite playerSprite ;
 
 bool onGround=false ;
 bool isDropping=false ;
 int dropTimer=0 ;
 
 // slide variables for slopes
 bool isSliding=false ;
 int slideDir=0 ;
 
 float velocityY=0 ;
 float terminalVelocity=12 ;
 
 // animation counters
 int animCounter=0 ;
 int skeletonAnimCounter[4]={0,0,0,0} ;
 int ghostTurnCounter=0 ;
 int damageTimer=0 ;
 // loading player textures based on selection
 if(selectedPlayer==0)
 {
  if(playerMoveTexture.loadFromFile("Assets/player1movement.png")==false)
   playerMoveTexture.loadFromFile("Assets/player1.png") ;
  playerIdleTexture.loadFromFile("Assets/player1.png") ;
  if(playerJumpTexture.loadFromFile("Assets/player1jump.png")==false)
   playerJumpTexture.loadFromFile("Assets/player1.png") ;
 }
 else
 {
  if(playerMoveTexture.loadFromFile("Assets/player2movement.png")==false)
   playerMoveTexture.loadFromFile("Assets/player2.png") ;
  playerIdleTexture.loadFromFile("Assets/player2.png") ;
  if(playerJumpTexture.loadFromFile("Assets/player2jump.png")==false)
   playerJumpTexture.loadFromFile("Assets/player2.png") ;
 }
 
 playerSprite.setTexture(playerIdleTexture) ;
 
 // player animation frame data
 int frameWidth=32 ;
 int frameHeight=40 ;
 int maxFrames=4 ;
 int currentFrame=0 ;
 int idleWidth=32 ;
 int idleHeight=40 ;
 int jumpWidth=32 ;
 int jumpHeight=40 ;
 int maxJumpFrames=1 ;
 
 bool isMoving=false ;
 bool facingRight=true ;
 
 playerSprite.setTextureRect(IntRect(0,0,idleWidth,idleHeight)) ;
 playerSprite.setScale(2,2) ;
 
 // player size and position
 float playerWidth=idleWidth*2.0f ;
 float playerHeight=42*2.0f ; // actual image height=42
 
 float playerX=5*CELL_SIZE ;
 float playerY=(MAP_HEIGHT-1)*CELL_SIZE-playerHeight ;
 float spawnX=playerX ;
 float spawnY=playerY ;
 
 // skeleton enemy setup
 const int NUM_SKELETONS=4 ;
 float skeletonX[4],skeletonY[4] ;
 int skeletonDir[4] ;
 bool skeletonActive[4] ;
 int skeletonState[4],skeletonFrame[4] ;
 float skeletonVelocityY[4] ;
 int skeletonMoveCounter[4] ;
 
 // loading skeleton textures
 Texture skeletonIdleTex,skeletonRunTex,skeletonTurnTex,skeletonJumpTex ;
 skeletonIdleTex.loadFromFile("Assets/skeleton.png") ;
 skeletonRunTex.loadFromFile("Assets/skeletonmove.png") ;
 skeletonTurnTex.loadFromFile("Assets/skeletonside.png") ;
 skeletonJumpTex.loadFromFile("Assets/skeltonjump.png") ;
 
 Sprite skeletonSprite ;
 skeletonSprite.setScale(2,2) ;
 
 // skeleton frame data
 int skelRunWidth=32,skelRunHeight=36 ;
 int skelJumpWidth=33,skelJumpHeight=47 ;
 float skeletonWidth=32*2.0f ;
 float skeletonHeight=36*2.0f ; // actual image height=36
 float skeletonSpeed=1.0f ;
 
 // initializing skeleton data
 for(int i=0;i<NUM_SKELETONS;i++)
 {
  skeletonActive[i]=true ;
  skeletonState[i]=1 ; // 1=run state
  skeletonFrame[i]=0 ;
  skeletonAnimCounter[i]=0 ;
  skeletonVelocityY[i]=0 ;
  skeletonMoveCounter[i]=0 ;
 }
 
 // ghost enemy setup
 const int NUM_GHOSTS=8 ;
 float ghostX[8],ghostY[8] ;
 int ghostDir[8] ;
 bool ghostActive[8],ghostTurning[8] ;
 int ghostTurnFrame[8] ;
 
 // loading ghost textures
 Texture ghostNormalTex,ghostTurnTex ;
 ghostNormalTex.loadFromFile("Assets/ghost.png") ;
 ghostTurnTex.loadFromFile("Assets/ghostside.png") ;
 
 Sprite ghostSprite ;
 ghostSprite.setScale(2,2) ;
 
 // ghost frame data
 int ghostNormalWidth=32,ghostNormalHeight=36 ;
 int ghostTurnWidth=40,ghostTurnHeight=36 ;
 float ghostWidth=ghostNormalWidth*1.5f ;
 float ghostHeight=ghostNormalHeight*1.6f ;
 float ghostSpeed=1.5f ;
 
 // initializing ghost data
 for(int i=0;i<NUM_GHOSTS;i++)
 {
  ghostTurning[i]=false ;
  ghostTurnFrame[i]=0 ;
  ghostActive[i]=true ;
 }
 
 // captured enemies setup
 const int MAX_CAPTURED=3 ;
 float capturedX[5],capturedY[5] ;
 int capturedType[5],capturedDir[5] ;
 bool capturedActive[5] ;
 time_t capturedTime[5] ;
 int capturedFrame[5],capturedAnimCounter[5] ;
 int captureQueue[5] ;
 
 // loading vacuum textures
 Texture vacuumRightTex,vacuumLeftTex,vacuumUpTex,vacuumDownTex ;
 Sprite vacuumSprite ;
 
 bool showVacuum=false ;
 int vacuumDirection=0 ; // 0=right 1=left 2=up 3=down
 int vacuumFrame=0 ;
 int vacuumAnimCounter=0 ;
 
 // vacuum ray frame positions
 int rayFrameX[3]={0,49,98} ;
 int rayFrameWidth=49,rayFrameHeight=30 ;
 int rayVertY[3]={0,49,98} ;
 int rayVertWidth=30,rayVertHeight=49 ;
 
 float vacuumX=0,vacuumY=0 ;
 float vacuumRangeH=150.0f,vacuumRangeV=150.0f ;
 
 int numCaptured=0 ;
 bool isVacuuming=false ;
 
 bool xPressed=false,zPressed=false ;
 
 // capture animation arrays
 bool ghostBeingCaptured[8] ;
 bool skeletonBeingCaptured[4] ;
 int ghostCaptureFrame[8],skeletonCaptureFrame[4] ;
 int ghostCaptureTimer[8],skeletonCaptureTimer[4] ;
 
 // initializing capture arrays
 for(int i=0;i<8;i++)
 {
  ghostBeingCaptured[i]=false ;
  ghostCaptureFrame[i]=0 ;
  ghostCaptureTimer[i]=0 ;
 }
 for(int i=0;i<4;i++)
 {
  skeletonBeingCaptured[i]=false ;
  skeletonCaptureFrame[i]=0 ;
  skeletonCaptureTimer[i]=0 ;
 }
 
 const int CAPTURE_DURATION=45 ; // frames needed to capture enemy
 
 // loading captured enemy textures
 Texture ghostCapturedTex,skeletonCapturedTex,ghostShotTex,skeletonShotTex ;
 ghostCapturedTex.loadFromFile("Assets/ghostcaptured.png") ;
 skeletonCapturedTex.loadFromFile("Assets/skeletoncaptured.png") ;
 ghostShotTex.loadFromFile("Assets/ghostshot.png") ;
 skeletonShotTex.loadFromFile("Assets/skeletonshot.png") ;
 
 // loading vacuum ray textures
 vacuumRightTex.loadFromFile("Assets/vaccumright.png") ;
 vacuumLeftTex.loadFromFile("Assets/vaccumleft.png") ;
 vacuumUpTex.loadFromFile("Assets/vaccumup.png") ;
 vacuumDownTex.loadFromFile("Assets/vaccumdown.png") ;
 
 vacuumSprite.setTexture(vacuumRightTex) ;
 vacuumSprite.setScale(3,3) ;
 
 // captured enemy frame sizes
 int ghostCapW=130/3,ghostCapH=27 ;
 int skelCapW=112/3,skelCapH=30 ;
 int ghostShotW=230/7,ghostShotH=30 ;
 int skelShotW=230/7,skelShotH=30 ;
 
 // initializing captured enemy arrays
 for(int i=0;i<5;i++)
 {
  capturedX[i]=0 ;
  capturedY[i]=0 ;
  capturedType[i]=0 ;
  capturedActive[i]=false ;
  capturedDir[i]=1 ;
  capturedFrame[i]=0 ;
  capturedAnimCounter[i]=0 ;
  capturedTime[i]=0 ;
  captureQueue[i]=-1 ;
 }
 
 // player health and score
 int playerHealth=3 ;
 bool isInvulnerable=false ;
 
 // score constants
 const int SCORE_CAPTURE_GHOST=50 ;
 const int SCORE_CAPTURE_SKELETON=75 ;
 const int SCORE_DEFEAT_GHOST=100 ;
 const int SCORE_DEFEAT_SKELETON=150 ;
 const int SCORE_DOUBLE_KILL=200 ;
 const int SCORE_TRIPLE_KILL=500 ;
 const int PENALTY_HIT=-50 ;
 const int PENALTY_DEATH=-200 ;
 
 int killsThisFrame=0 ;
 
 srand(time(0)) ; // initializing random seed
 // creating level array
 levelMap=new char*[MAP_HEIGHT] ;
 for(int i=0;i<MAP_HEIGHT;i++)
 {
  levelMap[i]=new char[MAP_WIDTH] ;
 }
 
 int currentLevel=levelChoice ;
 
 // loading game from save or starting fresh
 if(loadFromSave==true)
 {
  loadGameState(currentLevel,playerX,playerY,playerHealth,playerScore,ghostX,ghostY,ghostDir,ghostActive,NUM_GHOSTS,skeletonX,skeletonY,skeletonDir,skeletonActive,skeletonState,NUM_SKELETONS,numCaptured,selectedPlayer) ;
  createPlatforms(levelMap,MAP_HEIGHT,MAP_WIDTH,currentLevel) ;
 }
 else
 {
  createPlatforms(levelMap,MAP_HEIGHT,MAP_WIDTH,currentLevel) ;
  
  // spawning skeletons on platforms
  for(int i=0;i<NUM_SKELETONS;i++)
  {
   spawnOnPlatform(levelMap,MAP_HEIGHT,MAP_WIDTH,CELL_SIZE,skeletonX[i],skeletonY[i],skeletonDir[i],skeletonHeight) ;
  }
  
  // spawning ghosts on platforms
  for(int i=0;i<NUM_GHOSTS;i++)
  {
   spawnOnPlatform(levelMap,MAP_HEIGHT,MAP_WIDTH,CELL_SIZE,ghostX[i],ghostY[i],ghostDir[i],ghostHeight) ;
  }
 }
 
 // creating score text
 Text scoreText ;
 scoreText.setFont(gameFont) ;
 scoreText.setCharacterSize(40) ;
 scoreText.setFillColor(whiteColor) ;
 scoreText.setPosition(450,15) ;
 
 // pause system variables
 bool isPaused=false ;
 int pauseSelection=0 ;
 const int PAUSE_OPTIONS=3 ;
 
 // creating pause menu texts
 Text pauseTitle ;
 pauseTitle.setFont(gameFont) ;
 pauseTitle.setString("PAUSED") ;
 pauseTitle.setCharacterSize(80) ;
 pauseTitle.setFillColor(goldColor) ;
 pauseTitle.setPosition(SCREEN_WIDTH/2-150,200) ;
 
 Text pauseResume ;
 pauseResume.setFont(gameFont) ;
 pauseResume.setString("RESUME") ;
 pauseResume.setCharacterSize(40) ;
 pauseResume.setFillColor(whiteColor) ;
 pauseResume.setPosition(SCREEN_WIDTH/2-80,350) ;
 
 Text pauseSave ;
 pauseSave.setFont(gameFont) ;
 pauseSave.setString("SAVE & EXIT") ;
 pauseSave.setCharacterSize(40) ;
 pauseSave.setFillColor(whiteColor) ;
 pauseSave.setPosition(SCREEN_WIDTH/2-120,430) ;
 
 Text pauseMenu ;
 pauseMenu.setFont(gameFont) ;
 pauseMenu.setString("MAIN MENU") ;
 pauseMenu.setCharacterSize(40) ;
 pauseMenu.setFillColor(whiteColor) ;
 pauseMenu.setPosition(SCREEN_WIDTH/2-110,510) ;
 
 bool levelComplete=false ; // level completion flag
 
 // main game loop starts here
 while(window.isOpen())
 {
  while(window.pollEvent(event))
  {
   if(event.type==Event::Closed)
   {
    saveGameState(currentLevel,playerX,playerY,playerHealth,playerScore,ghostX,ghostY,ghostDir,ghostActive,NUM_GHOSTS,skeletonX,skeletonY,skeletonDir,skeletonActive,skeletonState,NUM_SKELETONS,numCaptured,selectedPlayer) ;
    window.close() ;
   }
   
   // pause key pressed
   if(event.type==Event::KeyPressed && event.key.code==Keyboard::P)
   {
    isPaused=!isPaused ;
   }
   
   // pause menu navigation
   if(isPaused==true && event.type==Event::KeyPressed)
   {
    if(event.key.code==Keyboard::Up)
    {
     pauseSelection-- ;
     if(pauseSelection<0) pauseSelection=PAUSE_OPTIONS-1 ;
    }
    if(event.key.code==Keyboard::Down)
    {
     pauseSelection++ ;
     if(pauseSelection>=PAUSE_OPTIONS) pauseSelection=0 ;
    }
    if(event.key.code==Keyboard::Return)
    {
     if(pauseSelection==0) // resume
     {
      isPaused=false ;
     }
     else if(pauseSelection==1) // save and exit
     {
      saveGameState(currentLevel,playerX,playerY,playerHealth,playerScore,ghostX,ghostY,ghostDir,ghostActive,NUM_GHOSTS,skeletonX,skeletonY,skeletonDir,skeletonActive,skeletonState,NUM_SKELETONS,numCaptured,selectedPlayer) ;
      window.close() ;
     }
     else if(pauseSelection==2) // main menu
     {
      deleteSaveFile() ;
      window.close() ;
     }
    }
   }
  }
  
  // showing pause menu
  if(isPaused==true)
  {
   highlightText(pauseResume,pauseSelection==0,goldColor,whiteColor) ;
   highlightText(pauseSave,pauseSelection==1,goldColor,whiteColor) ;
   highlightText(pauseMenu,pauseSelection==2,goldColor,whiteColor) ;
   
   window.clear(Color::Black) ;
   
   RectangleShape overlay ;
   overlay.setSize(Vector2f(SCREEN_WIDTH,SCREEN_HEIGHT)) ;
   overlay.setFillColor(Color(0,0,0,180)) ;
   window.draw(overlay) ;
   window.draw(pauseTitle) ;
   window.draw(pauseResume) ;
   window.draw(pauseSave) ;
   window.draw(pauseMenu) ;
   window.display() ;
   continue ;
  }
  
  // game logic starts here
  if(Keyboard::isKeyPressed(Keyboard::Escape))
  {
   window.close() ;
  }
  
  isMoving=false ;
  
  // player movement left
  if(Keyboard::isKeyPressed(Keyboard::Left))
  {
   playerX-=moveSpeed ;
   isMoving=true ;
   facingRight=false ;
   if(playerX<0) playerX=0 ;
  }
  
  // player movement right
  if(Keyboard::isKeyPressed(Keyboard::Right))
  {
   playerX+=moveSpeed ;
   isMoving=true ;
   facingRight=true ;
   if(playerX+playerWidth>SCREEN_WIDTH) playerX=SCREEN_WIDTH-playerWidth ;
  }
  
  // player jump
  if(Keyboard::isKeyPressed(Keyboard::Up) && onGround==true)
  {
   velocityY=JUMP_STRENGTH ;
   onGround=false ;
  }
  
  // player drop through platform
  if(Keyboard::isKeyPressed(Keyboard::Down) && onGround==true)
  {
   isDropping=true ;
   dropTimer=15 ;
   onGround=false ;
   velocityY=2.0f ;
  }
  
  // updating drop timer
  if(isDropping==true)
  {
   dropTimer-- ;
   if(dropTimer<=0) isDropping=false ;
  }
  // vacuum controls
  if(Keyboard::isKeyPressed(Keyboard::Space))
  {
   isVacuuming=true ;
   showVacuum=true ;
   
   if(Keyboard::isKeyPressed(Keyboard::W)) // vacuum up
   {
    vacuumDirection=2 ;
    vacuumSprite.setTexture(vacuumUpTex) ;
   }
   else if(Keyboard::isKeyPressed(Keyboard::S)) // vacuum down
   {
    vacuumDirection=3 ;
    vacuumSprite.setTexture(vacuumDownTex) ;
   }
   else if(Keyboard::isKeyPressed(Keyboard::A)) // vacuum left
   {
    vacuumDirection=1 ;
    vacuumSprite.setTexture(vacuumLeftTex) ;
    facingRight=false ;
   }
   else if(Keyboard::isKeyPressed(Keyboard::D)) // vacuum right
   {
    vacuumDirection=0 ;
    vacuumSprite.setTexture(vacuumRightTex) ;
    facingRight=true ;
   }
   else // default vacuum direction
   {
    if(facingRight==true)
    {
     vacuumDirection=0 ;
     vacuumSprite.setTexture(vacuumRightTex) ;
    }
    else
    {
     vacuumDirection=1 ;
     vacuumSprite.setTexture(vacuumLeftTex) ;
    }
   }
  }
  else
  {
   isVacuuming=false ;
   showVacuum=false ;
   vacuumFrame=0 ;
   vacuumAnimCounter=0 ;
  }
  
  // shoot one enemy with X key
  if(Keyboard::isKeyPressed(Keyboard::X))
  {
   if(xPressed==false && numCaptured>0)
   {
    numCaptured-- ;
    capturedActive[numCaptured]=true ;
    capturedY[numCaptured]=playerY ;
    
    if(facingRight==true)
    {
     capturedX[numCaptured]=playerX+playerWidth+10 ;
     capturedDir[numCaptured]=1 ;
    }
    else
    {
     capturedX[numCaptured]=playerX-40 ;
     capturedDir[numCaptured]=-1 ;
    }
    
    capturedFrame[numCaptured]=0 ;
    capturedTime[numCaptured]=time(0) ;
    xPressed=true ;
   }
  }
  else
  {
   xPressed=false ;
  }
  
  // shoot all enemies with Z key
  if(Keyboard::isKeyPressed(Keyboard::Z))
  {
   if(zPressed==false && numCaptured>0)
   {
    for(int i=0;i<numCaptured;i++)
    {
     capturedActive[i]=true ;
     capturedY[i]=playerY ;
     
     if(facingRight==true)
     {
      capturedX[i]=playerX+playerWidth+10 ;
      capturedDir[i]=1 ;
     }
     else
     {
      capturedX[i]=playerX-40 ;
      capturedDir[i]=-1 ;
     }
     
     capturedFrame[i]=0 ;
     capturedTime[i]=time(0) ;
    }
    
    numCaptured=0 ;
    zPressed=true ;
   }
  }
  else
  {
   zPressed=false ;
  }
  
  // applying gravity to player
  player_gravity(levelMap,velocityY,onGround,GRAVITY,terminalVelocity,playerX,playerY,CELL_SIZE,MAP_HEIGHT,MAP_WIDTH,playerWidth,playerHeight,isDropping,SCREEN_HEIGHT) ;
  
  // checking if player is on slope
  isSliding=false ;
  slideDir=0 ;
  
  int playerCol=(int)((playerX+playerWidth/2)/CELL_SIZE) ;
  int playerRow=(int)((playerY+playerHeight+1)/CELL_SIZE) ;
  
  if(playerRow>=0 && playerRow<MAP_HEIGHT && playerCol>=0 && playerCol<MAP_WIDTH)
  {
   char tile=levelMap[playerRow][playerCol] ;
   
   if(tile=='>') // right slope
   {
    isSliding=true ;
    slideDir=1 ; // slide right
   }
   else if(tile=='<') // left slope
   {
    isSliding=true ;
    slideDir=-1 ; // slide left
   }
  }
  
  // applying slide movement
  if(isSliding==true && onGround==true)
  {
   playerX+=slideDir*3.0f ; // move sideways
   velocityY=1.5f ; // push down slowly
  }
  
  // preventing player from going off top of screen
  if(playerY<0)
  {
   playerY=0 ;
   velocityY=0 ;
  }
  
  // flipping player sprite based on direction
  if(facingRight==true) playerSprite.setScale(-2,2) ;
  else playerSprite.setScale(2,2) ;
  
  // updating player animation
  updateAnimation(playerSprite,currentFrame,maxFrames,animCounter,frameWidth,frameHeight,isMoving,facingRight,playerMoveTexture,playerIdleTexture,idleWidth,idleHeight,playerJumpTexture,jumpWidth,jumpHeight,maxJumpFrames,onGround) ;
  
  playerSprite.setPosition(playerX,playerY) ;
  
  // updating ghost enemies
  updateGhosts(ghostX,ghostY,ghostDir,ghostActive,ghostTurning,levelMap,CELL_SIZE,MAP_WIDTH,ghostSpeed,NUM_GHOSTS,ghostWidth,ghostHeight,SCREEN_WIDTH) ;
  
  // updating skeleton enemies
  updateSkeletons(skeletonX,skeletonY,skeletonDir,skeletonActive,skeletonState,skeletonFrame,skeletonAnimCounter,skeletonVelocityY,skeletonMoveCounter,levelMap,CELL_SIZE,MAP_WIDTH,MAP_HEIGHT,skeletonSpeed,NUM_SKELETONS,skeletonWidth,skeletonHeight,GRAVITY,terminalVelocity) ;
  
  // updating vacuum ray animation
  if(showVacuum==true)
  {
   vacuumAnimCounter++ ;
   
   if(vacuumAnimCounter>=6)
   {
    vacuumFrame++ ;
    if(vacuumFrame>=3) vacuumFrame=0 ;
    vacuumAnimCounter=0 ;
   }
   
   // calculating vacuum position based on direction
   float rayW=rayFrameWidth*3 ;
   float rayH=rayFrameHeight*3 ;
   float rayVW=rayVertWidth*3 ;
   float rayVH=rayVertHeight*3 ;
   float handX=playerX+32 ;
   float handY=playerY+35 ;
   
   if(vacuumDirection==0) // right
   {
    vacuumX=playerX+20 ;
    vacuumY=handY-rayH/2 ;
   }
   else if(vacuumDirection==1) // left
   {
    vacuumX=playerX-rayW+10 ;
    vacuumY=handY-rayH/2 ;
   }
   else if(vacuumDirection==2) // up
   {
    vacuumX=handX-rayVW/2 ;
    vacuumY=playerY-rayVH+15 ;
   }
   else // down
   {
    vacuumX=handX-rayVW/2 ;
    vacuumY=playerY+70 ;
   }
  }
  // vacuum capture logic
  if(isVacuuming==true && numCaptured<MAX_CAPTURED)
  {
   // checking if ghosts are in vacuum range
   for(int i=0;i<NUM_GHOSTS;i++)
   {
    if(ghostActive[i]==true && ghostBeingCaptured[i]==false)
    {
     if(isInVacuumRange(ghostX[i],ghostY[i],playerX,playerY,playerWidth,playerHeight,vacuumDirection,vacuumRangeH,vacuumRangeV)==true)
     {
      ghostBeingCaptured[i]=true ;
      ghostCaptureFrame[i]=0 ;
      ghostCaptureTimer[i]=0 ;
      break ;
     }
    }
   }
   
   // checking if skeletons are in vacuum range
   for(int i=0;i<NUM_SKELETONS;i++)
   {
    if(skeletonActive[i]==true && skeletonBeingCaptured[i]==false)
    {
     if(isInVacuumRange(skeletonX[i],skeletonY[i],playerX,playerY,playerWidth,playerHeight,vacuumDirection,vacuumRangeH,vacuumRangeV)==true)
     {
      skeletonBeingCaptured[i]=true ;
      skeletonCaptureFrame[i]=0 ;
      skeletonCaptureTimer[i]=0 ;
      break ;
     }
    }
   }
  }
  
  // updating ghost capture animation
  for(int i=0;i<NUM_GHOSTS;i++)
  {
   if(ghostBeingCaptured[i]==true)
   {
    ghostCaptureTimer[i]++ ;
    
    // updating capture animation frame
    if(ghostCaptureTimer[i]%25==0)
    {
     ghostCaptureFrame[i]++ ;
     if(ghostCaptureFrame[i]>=3) ghostCaptureFrame[i]=0 ;
    }
    
    // pulling ghost towards player
    float dx=(playerX+playerWidth/2)-ghostX[i] ;
    float dy=(playerY+playerHeight/2)-ghostY[i] ;
    ghostX[i]+=dx*0.6f ;
    ghostY[i]+=dy*0.6f ;
    
    // capture complete
    if(ghostCaptureTimer[i]>=CAPTURE_DURATION)
    {
     ghostActive[i]=false ;
     ghostBeingCaptured[i]=false ;
     
     if(numCaptured<MAX_CAPTURED)
     {
      capturedType[numCaptured]=0 ; // 0=ghost
      captureQueue[numCaptured]=numCaptured ;
      numCaptured++ ;
      playerScore+=SCORE_CAPTURE_GHOST ;
     }
    }
   }
  }
  
  // updating skeleton capture animation
  for(int i=0;i<NUM_SKELETONS;i++)
  {
   if(skeletonBeingCaptured[i]==true)
   {
    skeletonCaptureTimer[i]++ ;
    
    // updating capture animation frame
    if(skeletonCaptureTimer[i]%25==0)
    {
     skeletonCaptureFrame[i]++ ;
     if(skeletonCaptureFrame[i]>=3) skeletonCaptureFrame[i]=0 ;
    }
    
    // pulling skeleton towards player
    float dx=(playerX+playerWidth/2)-skeletonX[i] ;
    float dy=(playerY+playerHeight/2)-skeletonY[i] ;
    skeletonX[i]+=dx*0.6f ;
    skeletonY[i]+=dy*0.6f ;
    
    // capture complete
    if(skeletonCaptureTimer[i]>=CAPTURE_DURATION)
    {
     skeletonActive[i]=false ;
     skeletonBeingCaptured[i]=false ;
     
     if(numCaptured<MAX_CAPTURED)
     {
      capturedType[numCaptured]=1 ; // 1=skeleton
      captureQueue[numCaptured]=numCaptured ;
      numCaptured++ ;
      playerScore+=SCORE_CAPTURE_SKELETON ;
     }
    }
   }
  }
  
  // updating ghost turn animation
  ghostTurnCounter++ ;
  if(ghostTurnCounter>=30)
  {
   for(int i=0;i<NUM_GHOSTS;i++)
   {
    if(ghostTurning[i]==true)
    {
     ghostTurnFrame[i]++ ;
     
     if(ghostTurnFrame[i]>=2)
     {
      ghostTurning[i]=false ;
      ghostTurnFrame[i]=0 ;
     }
    }
   }
   ghostTurnCounter=0 ;
  }
  
  // updating shot enemies
  killsThisFrame=0 ;
  
  for(int i=0;i<5;i++)
  {
   if(capturedActive[i]==true)
   {
    capturedAnimCounter[i]++ ;
    
    // updating shot animation frame
    if(capturedAnimCounter[i]>=25)
    {
     capturedFrame[i]++ ;
     if(capturedFrame[i]>=5) capturedFrame[i]=0 ;
     capturedAnimCounter[i]=0 ;
    }
    
    // moving shot enemy
    if(capturedDir[i]==1) capturedX[i]+=9.0f ;
    else capturedX[i]-=9.0f ;
    
    // removing shot enemy after 3 seconds
    if(time(0)-capturedTime[i]>=3)
    {
     capturedActive[i]=false ;
     continue ;
    }
    
    // checking collision with ghosts
    for(int j=0;j<NUM_GHOSTS;j++)
    {
     if(ghostActive[j]==true)
     {
      if(checkCollision(capturedX[i],capturedY[i],66,60,ghostX[j],ghostY[j],ghostWidth,ghostHeight)==true)
      {
       ghostActive[j]=false ;
       capturedActive[i]=false ;
       playerScore+=SCORE_DEFEAT_GHOST ;
       killsThisFrame++ ;
       break ;
      }
     }
    }
    
    // checking collision with skeletons
    for(int j=0;j<NUM_SKELETONS;j++)
    {
     if(skeletonActive[j]==true)
     {
      if(checkCollision(capturedX[i],capturedY[i],66,60,skeletonX[j],skeletonY[j],skeletonWidth,skeletonHeight)==true)
      {
       skeletonActive[j]=false ;
       capturedActive[i]=false ;
       playerScore+=SCORE_DEFEAT_SKELETON ;
       killsThisFrame++ ;
       break ;
      }
     }
    }
    
    // removing shot enemy if off screen
    if(capturedX[i]<-50 || capturedX[i]>SCREEN_WIDTH+50)
    {
     capturedActive[i]=false ;
    }
   }
  }
  
  // multi-kill bonus
  if(killsThisFrame>=3) playerScore+=SCORE_TRIPLE_KILL ;
  else if(killsThisFrame==2) playerScore+=SCORE_DOUBLE_KILL ;
  // checking if all enemies are dead
  bool allDead=true ;
  
  for(int i=0;i<NUM_GHOSTS;i++)
  {
   if(ghostActive[i]==true)
   {
    allDead=false ;
    break ;
   }
  }
  
  if(allDead==true)
  {
   for(int i=0;i<NUM_SKELETONS;i++)
   {
    if(skeletonActive[i]==true)
    {
     allDead=false ;
     break ;
    }
   }
  }
  
  // waiting for all captures to finish
  if(allDead==true)
  {
   for(int i=0;i<NUM_GHOSTS;i++)
   {
    if(ghostBeingCaptured[i]==true)
    {
     allDead=false ;
     break ;
    }
   }
  }
  
  if(allDead==true)
  {
   for(int i=0;i<NUM_SKELETONS;i++)
   {
    if(skeletonBeingCaptured[i]==true)
    {
     allDead=false ;
     break ;
    }
   }
  }
  
  // level complete
  if(allDead==true && levelComplete==false)
  {
   levelComplete=true ;
  }
  
  // showing level complete screen
  if(levelComplete==true)
  {
   window.clear(Color(20,20,50)) ;
   
   // drawing title
   Text completeTitle ;
   completeTitle.setFont(gameFont) ;
   completeTitle.setString("LEVEL COMPLETE!") ;
   completeTitle.setCharacterSize(70) ;
   completeTitle.setFillColor(goldColor) ;
   completeTitle.setPosition(SCREEN_WIDTH/2-280,150) ;
   window.draw(completeTitle) ;
   
   // converting score to string
   char scoreStr[30]="Score: " ;
   int s=playerScore,p=7 ;
   if(s==0) scoreStr[p++]='0' ;
   else
   {
    char d[10] ;
    int dc=0 ;
    while(s>0)
    {
     d[dc++]='0'+s%10 ;
     s/=10 ;
    }
    while(dc>0) scoreStr[p++]=d[--dc] ;
   }
   scoreStr[p]='\0' ;
   
   // drawing score
   Text scoreT ;
   scoreT.setFont(gameFont) ;
   scoreT.setString(scoreStr) ;
   scoreT.setCharacterSize(50) ;
   scoreT.setFillColor(whiteColor) ;
   scoreT.setPosition(SCREEN_WIDTH/2-100,280) ;
   window.draw(scoreT) ;
   
   // drawing name prompt
   Text prompt ;
   prompt.setFont(gameFont) ;
   prompt.setString("Enter Name:") ;
   prompt.setCharacterSize(40) ;
   prompt.setFillColor(whiteColor) ;
   prompt.setPosition(SCREEN_WIDTH/2-120,380) ;
   window.draw(prompt) ;
   
   // drawing name input with cursor
   char nameShow[20] ;
   for(int i=0;i<inputNameLength;i++) nameShow[i]=inputName[i] ;
   nameShow[inputNameLength]='_' ;
   nameShow[inputNameLength+1]='\0' ;
   
   Text nameT ;
   nameT.setFont(gameFont) ;
   nameT.setString(nameShow) ;
   nameT.setCharacterSize(50) ;
   nameT.setFillColor(yellowColor) ;
   nameT.setPosition(SCREEN_WIDTH/2-80,450) ;
   window.draw(nameT) ;
   
   // drawing hint
   Text hint ;
   hint.setFont(gameFont) ;
   hint.setString("Type name + ENTER") ;
   hint.setCharacterSize(25) ;
   hint.setFillColor(grayColor) ;
   hint.setPosition(SCREEN_WIDTH/2-120,530) ;
   window.draw(hint) ;
   
   window.display() ;
   
   // handling input for name entry
   Event e ;
   while(window.pollEvent(e))
   {
    if(e.type==Event::Closed) window.close() ;
    
    if(e.type==Event::KeyPressed)
    {
     if(e.key.code==Keyboard::Return && inputNameLength>0)
     {
      addScoreToLeaderboard(leaderboardNames,leaderboardScores,leaderboardCount,inputName,playerScore) ;
      window.close() ;
     }
     if(e.key.code==Keyboard::BackSpace && inputNameLength>0)
     {
      inputNameLength-- ;
      inputName[inputNameLength]='\0' ;
     }
    }
    
    if(e.type==Event::TextEntered)
    {
     char c=e.text.unicode ;
     if(((c>='A' && c<='Z') || (c>='a' && c<='z')) && inputNameLength<14)
     {
      inputName[inputNameLength++]=c ;
      inputName[inputNameLength]='\0' ;
     }
    }
   }
   continue ;
  }
  
  // damage timer for invulnerability
  if(isInvulnerable==true)
  {
   damageTimer++ ;
   if(damageTimer>=120)
   {
    isInvulnerable=false ;
    damageTimer=0 ;
   }
  }
  
  // checking collision with ghosts
  if(isInvulnerable==false)
  {
   for(int i=0;i<NUM_GHOSTS;i++)
   {
    if(ghostActive[i]==true && ghostBeingCaptured[i]==false)
    {
     if(checkCollision(playerX,playerY,playerWidth,playerHeight,ghostX[i],ghostY[i],ghostWidth,ghostHeight)==true)
     {
      playerHealth-- ;
      isInvulnerable=true ;
      damageTimer=0 ;
      playerScore+=PENALTY_HIT ;
      playerX=spawnX ;
      playerY=spawnY ;
      velocityY=0 ;
      
      if(playerHealth<=0)
      {
       playerScore+=PENALTY_DEATH ;
       levelComplete=true ;
       inputNameLength=0 ;
       for(int k=0;k<15;k++) inputName[k]='\0' ;
      }
      break ;
     }
    }
   }
   
   // checking collision with skeletons
   for(int i=0;i<NUM_SKELETONS;i++)
   {
    if(skeletonActive[i]==true && skeletonBeingCaptured[i]==false)
    {
     if(checkCollision(playerX,playerY,playerWidth,playerHeight,skeletonX[i],skeletonY[i],skeletonWidth,skeletonHeight)==true)
     {
      playerHealth-- ;
      isInvulnerable=true ;
      damageTimer=0 ;
      playerScore+=PENALTY_HIT ;
      playerX=spawnX ;
      playerY=spawnY ;
      velocityY=0 ;
      
      if(playerHealth<=0)
      {
       playerScore+=PENALTY_DEATH ;
       levelComplete=true ;
       inputNameLength=0 ;
       for(int k=0;k<15;k++) inputName[k]='\0' ;
      }
      break ;
     }
    }
   }
  }
  // rendering starts here
  window.clear() ;
  
  // drawing level with platforms and slopes
  display_level(window,levelMap,backgroundTexture,backgroundSprite,blockTexture,blockSprite,slopeRightTex,slopeLeftTex,slopeSprite,MAP_HEIGHT,MAP_WIDTH,CELL_SIZE) ;
  
  // drawing ghosts
  for(int i=0;i<NUM_GHOSTS;i++)
  {
   if(ghostActive[i]==true || ghostBeingCaptured[i]==true)
   {
    if(ghostBeingCaptured[i]==true) // being captured animation
    {
     ghostSprite.setTexture(ghostCapturedTex) ;
     IntRect rect(ghostCaptureFrame[i]*ghostCapW,0,ghostCapW,ghostCapH) ;
     ghostSprite.setTextureRect(rect) ;
     ghostSprite.setScale(2,2) ;
     ghostSprite.setPosition(ghostX[i],ghostY[i]) ;
    }
    else if(ghostTurning[i]==true) // turning animation
    {
     ghostSprite.setTexture(ghostTurnTex) ;
     IntRect rect(ghostTurnFrame[i]*ghostTurnWidth,0,ghostTurnWidth,ghostTurnHeight) ;
     ghostSprite.setTextureRect(rect) ;
     
     if(ghostDir[i]==1)
     {
      ghostSprite.setScale(-2,2) ;
      ghostSprite.setPosition(ghostX[i]+ghostTurnWidth*2,ghostY[i]) ;
     }
     else
     {
      ghostSprite.setScale(2,2) ;
      ghostSprite.setPosition(ghostX[i],ghostY[i]) ;
     }
    }
    else // normal movement
    {
     ghostSprite.setTexture(ghostNormalTex) ;
     IntRect rect(0,0,ghostNormalWidth,ghostNormalHeight) ;
     ghostSprite.setTextureRect(rect) ;
     
     if(ghostDir[i]==1)
     {
      ghostSprite.setScale(-2,2) ;
      ghostSprite.setPosition(ghostX[i]+ghostWidth,ghostY[i]) ;
     }
     else
     {
      ghostSprite.setScale(2,2) ;
      ghostSprite.setPosition(ghostX[i],ghostY[i]) ;
     }
    }
    window.draw(ghostSprite) ;
   }
  }
  
  // drawing skeletons
  for(int i=0;i<NUM_SKELETONS;i++)
  {
   if(skeletonActive[i]==true || skeletonBeingCaptured[i]==true)
   {
    float drawX=skeletonX[i] ;
    float drawY=skeletonY[i] ;
    
    if(skeletonBeingCaptured[i]==true) // being captured animation
    {
     skeletonSprite.setTexture(skeletonCapturedTex) ;
     IntRect rect(skeletonCaptureFrame[i]*skelCapW,0,skelCapW,skelCapH) ;
     skeletonSprite.setTextureRect(rect) ;
     skeletonSprite.setScale(2,2) ;
     skeletonSprite.setPosition(drawX,drawY) ;
    }
    else if(skeletonState[i]==0) // idle state
    {
     skeletonSprite.setTexture(skeletonIdleTex) ;
     IntRect rect(0,0,skelRunWidth,skelRunHeight) ;
     skeletonSprite.setTextureRect(rect) ;
     
     if(skeletonDir[i]==1)
     {
      skeletonSprite.setScale(-2,2) ;
      skeletonSprite.setPosition(drawX+skelRunWidth*2,drawY) ;
     }
     else
     {
      skeletonSprite.setScale(2,2) ;
      skeletonSprite.setPosition(drawX,drawY) ;
     }
    }
    else if(skeletonState[i]==1) // run state
    {
     skeletonSprite.setTexture(skeletonRunTex) ;
     IntRect rect(skeletonFrame[i]*skelRunWidth,0,skelRunWidth,skelRunHeight) ;
     skeletonSprite.setTextureRect(rect) ;
     
     if(skeletonDir[i]==1)
     {
      skeletonSprite.setScale(-2,2) ;
      skeletonSprite.setPosition(drawX+skelRunWidth*2,drawY) ;
     }
     else
     {
      skeletonSprite.setScale(2,2) ;
      skeletonSprite.setPosition(drawX,drawY) ;
     }
    }
    else if(skeletonState[i]==2) // turn state
    {
     skeletonSprite.setTexture(skeletonTurnTex) ;
     IntRect rect(skeletonFrame[i]*skelRunWidth,0,skelRunWidth,skelRunHeight) ;
     skeletonSprite.setTextureRect(rect) ;
     
     if(skeletonDir[i]==1)
     {
      skeletonSprite.setScale(-2,2) ;
      skeletonSprite.setPosition(drawX+skelRunWidth*2,drawY) ;
     }
     else
     {
      skeletonSprite.setScale(2,2) ;
      skeletonSprite.setPosition(drawX,drawY) ;
     }
    }
    else if(skeletonState[i]==3) // jump state
    {
     skeletonSprite.setTexture(skeletonJumpTex) ;
     IntRect rect(0,0,skelJumpWidth,skelJumpHeight) ;
     skeletonSprite.setTextureRect(rect) ;
     drawY=skeletonY[i]-(skelJumpHeight-skelRunHeight) ;
     
     if(skeletonDir[i]==1)
     {
      skeletonSprite.setScale(-2,2) ;
      skeletonSprite.setPosition(drawX+skelJumpWidth*2,drawY) ;
     }
     else
     {
      skeletonSprite.setScale(2,2) ;
      skeletonSprite.setPosition(drawX,drawY) ;
     }
    }
    window.draw(skeletonSprite) ;
   }
  }
  
  // drawing shot enemies
  Sprite projectileSprite ;
  for(int i=0;i<5;i++)
  {
   if(capturedActive[i]==true)
   {
    if(capturedType[i]==0) // ghost projectile
    {
     projectileSprite.setTexture(ghostShotTex) ;
     IntRect rect(capturedFrame[i]*ghostShotW,0,ghostShotW,ghostShotH) ;
     projectileSprite.setTextureRect(rect) ;
    }
    else // skeleton projectile
    {
     projectileSprite.setTexture(skeletonShotTex) ;
     IntRect rect(capturedFrame[i]*skelShotW,0,skelShotW,skelShotH) ;
     projectileSprite.setTextureRect(rect) ;
    }
    
    projectileSprite.setScale(2,2) ;
    
    if(capturedDir[i]==1)
    {
     projectileSprite.setScale(-2,2) ;
     projectileSprite.setPosition(capturedX[i]+66,capturedY[i]) ;
    }
    else
    {
     projectileSprite.setPosition(capturedX[i],capturedY[i]) ;
    }
    window.draw(projectileSprite) ;
   }
  }
  
  // drawing vacuum ray
  if(showVacuum==true)
  {
   if(vacuumDirection==0 || vacuumDirection==1) // horizontal ray
   {
    IntRect rect(rayFrameX[vacuumFrame],0,rayFrameWidth,rayFrameHeight) ;
    vacuumSprite.setTextureRect(rect) ;
   }
   else // vertical ray
   {
    IntRect rect(0,rayVertY[vacuumFrame],rayVertWidth,rayVertHeight) ;
    vacuumSprite.setTextureRect(rect) ;
   }
   vacuumSprite.setScale(3,3) ;
   vacuumSprite.setPosition(vacuumX,vacuumY) ;
   window.draw(vacuumSprite) ;
  }
  
  // drawing player with flashing when invulnerable
  if(isInvulnerable==false || (damageTimer/6)%2==0)
  {
   window.draw(playerSprite) ;
  }
  
  // drawing health hearts
  for(int i=0;i<playerHealth;i++)
  {
   heartSprite.setPosition(50+i*60,30) ;
   window.draw(heartSprite) ;
  }
  
  // drawing score
  char scoreDisplay[20]="SCORE: " ;
  int tempScore=playerScore ;
  int pos=7 ;
  
  if(tempScore==0)
  {
   scoreDisplay[pos++]='0' ;
  }
  else
  {
   char digits[10] ;
   int dc=0 ;
   while(tempScore>0)
   {
    digits[dc++]='0'+tempScore%10 ;
    tempScore/=10 ;
   }
   while(dc>0) scoreDisplay[pos++]=digits[--dc] ;
  }
  scoreDisplay[pos]='\0' ;
  
  scoreText.setString(scoreDisplay) ;
  window.draw(scoreText) ;
  
  // drawing capture counter
  Text captureText ;
  captureText.setFont(gameFont) ;
  captureText.setCharacterSize(30) ;
  captureText.setFillColor(yellowColor) ;
  captureText.setPosition(900,20) ;
  
  char captureStr[20]="CAPTURED: " ;
  captureStr[10]='0'+numCaptured ;
  captureStr[11]='/' ;
  captureStr[12]='3' ;
  captureStr[13]='\0' ;
  captureText.setString(captureStr) ;
  window.draw(captureText) ;
  
  window.display() ;
 }
 
 // cleanup and stopping music
 gameMusic.stop() ;
 
 // deleting level array
 for(int i=0;i<MAP_HEIGHT;i++)
 {
  delete[] levelMap[i] ;
 }
 delete[] levelMap ;
 
 return 0 ;
}









































































































































































































































































































































































































































































/*
program ends here menu game mein chalti he , level complete screen pe name enter karne ka option he , game pehle start hoti he phir menu dikhti he   
*/