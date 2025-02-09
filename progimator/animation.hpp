//
//  animation.hpp
//  bitmem
//
//  Created by Samantha on 2/9/25.
//

#ifndef animation_hpp
#define animation_hpp

#include <stdio.h>
#include <vector>
#include <string>

class Animation
{
public:
    Animation(std::string filename);
    bool readFile(std::string filename);
    short getNumFrames(){ return mNumFrames; }
    short getFrameMS(){ return mFrameMS; }
    short getRowLength(){ return mRowLength; }
    short getFrameBytes() { return mRowLength * mDimensions[1]; }
    
    const std::string getDimString();
    const std::string getFrameString(int frame_no);
    
private:
    short mNumFrames;
    short mDimensions[2];
    short mFrameMS;
    int mRowLength;
    int mFrameSize;
    std::string mFilename;
    
    //Fancy new style replacement for typedefs
    using frame_data = std::vector<std::vector<unsigned char>>;
    frame_data mFrameData;
};

#endif /* animation_hpp */
