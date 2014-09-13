#include "Target.h"

Target::Target(){
  horizontalParticle = null;
  
  totalScore = 0;
  leftScore = 0;
  rightScore = 0;
  tapeWidthScore = 0;
  verticalScore = 0;
  
  hot = false;
}

void Target::checkIfHot(){
  hot = tapeWidthScore >= TAPE_WIDTH_LIMIT & verticalScore >= VERTICAL_SCORE_LIMIT & ((leftScore > LR_SCORE_LIMIT) | (rightScore > LR_SCORE_LIMIT));
}
