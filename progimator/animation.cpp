//
//  animation.cpp
//  bitmem
//
//  Created by Samantha on 2/9/25.
//

#include <fstream>
#include <cmath>
#include <iostream>
#include <sstream>
#include "animation.hpp"

Animation::Animation(std::string filename)
{
    mFilename = filename;
    if (!readFile(filename))
    {
        exit(0);
        //throw std::runtime_error("Was unable to open file " + filename );
    }
}

bool Animation::readFile(std::string filename)
{
    //Format is:  width, height, frameMS, num_frames, frame_data
    std::ifstream infile(filename, std::ios::out | std::ios::binary);
    
    if (!infile)
    {
        std::cout <<"Couldn't open file " << filename << std::endl;
        //Couldn't open file.
        return false;
    }
    
    infile.seekg (0, infile.end);
    long filesize = infile.tellg();
    infile.seekg (0, infile.beg);
        
    long headersize = 4*sizeof(short);
        
    if (filesize < headersize)
    {
        //File too short to even contain header.
        return false;
    }
    
    //Read the header:
    infile.read(reinterpret_cast<char*>(&mDimensions), sizeof(short) * 2);
    infile.read(reinterpret_cast<char*>(&mFrameMS), sizeof(short));
    infile.read(reinterpret_cast<char*>(&mNumFrames), sizeof(short));
    
    mRowLength = (short) ceil( (float)mDimensions[0] / 8.0 );
    mFrameSize = mRowLength*mDimensions[1];
    if (filesize < (headersize + mFrameSize*mNumFrames) )
    {
        return false;
    }
    
    for (int i=0;i<mNumFrames;++i)
    {
        std::vector<unsigned char> frame;
        frame.resize(mFrameSize);
        infile.read(reinterpret_cast<char*>(frame.data()), mFrameSize);
        mFrameData.push_back(frame);
    }
    
    infile.close();
    
    return true;
}

const std::string Animation::getDimString()
{
    std::ostringstream outstr;
    outstr << "{ " << mDimensions[0] << ", " << mDimensions[1] << " }";
    return outstr.str();
}

const std::string Animation::getFrameString(int frame_no)
{
    std::ostringstream outstr;
    
    outstr << "\t{";
    
    std::vector<unsigned char> frame = mFrameData[frame_no];
    
    int total = (int)frame.size();
    int last_comma = total - 1;
    
    for (int i=0;i<total;++i)
    {
        outstr << "0x" << std::hex << std::setw(2)
        << std::setfill('0') << static_cast<int>(frame[i]);
        if ( i < last_comma )
        {
            outstr << ", ";
        }
    }
    outstr << " }";
    
    return outstr.str();
}
