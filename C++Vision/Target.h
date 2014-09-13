#ifindef TARGET_H
#define TARGET_H

#define TAPE_WIDTH_LIMIT 50
#define VERTICAL_SCORE_LIMIT 50
#define LR_SCORE_LIMIT 50

class Target(){
  
  private:
  
    //The horizontal particle making up this target
    Particle horizontalParticle;
    
    //The vertical particle makig up this target
    Particle verticalParticle;
    
    int totalScore;
    int leftScore;
    int rightScore;
    int tapeWidthScore;
    int verticalScore;
    
    bool hot = false;
  
  public:
    
    //Default constructor
    Target();
    
    //Checks if this target is hot
    void checkIfHot();
}
