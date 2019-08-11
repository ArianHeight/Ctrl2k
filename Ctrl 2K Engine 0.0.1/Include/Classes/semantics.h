#pragma once

#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s, m) ((char*)NULL + (offsetof(s, m)))

//for gpu locations(Cg)
#define POSITION_ATTRIB 0
#define BLENDWEIGHT_ATTRIB 1
#define NORMAL_ATTRIB 2
#define COLOUR0_ATTRIB 3
#define DIFFUSE_ATTRIB 3
#define COLOUR1_ATTRIB 4
#define SPECULAR_ATTRIB 4
#define TESSFACTOR_ATTRIB 5
#define FOGCOORD_ATTRIB 5
#define PSIZE_ATTRIB 6
#define BLENDINDICES_ATTRIB 7
#define TEXCOORD0_ATTRIB 8
#define TEXCOORD1_ATTRIB 9
#define TEXCOORD2_ATTRIB 10
#define TEXCOORD3_ATTRIB 11
#define TEXCOORD4_ATTRIB 12
#define TEXCOORD5_ATTRIB 13
#define TEXCOORD6_ATTRIB 14
#define TANGENT_ATTRIB 14
#define TEXCOORD7_ATTRIB 15
#define BINORMAL_ATTRIB 15