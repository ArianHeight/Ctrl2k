#pragma once

GLuint loadTexture(const std::string& file, bool mips = true);

void getToken(std::vector<DataBlock> &out, std::string &line, std::string &work, int &index); //splits using space delimiter
void getTokenPath(std::string &out, std::string &line); //returns to out the name of the file

void loadOpenGLHexaBuffer(TriMesh *mesh, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &texcoords, std::vector<glm::vec3> &normals); //vertex, texcoord, normal, index, tangent, bitangent

void pathToSlash(std::string &path); //changes path to slash mode
void pathToBackSlash(std::string &path); //changes path to backslash mode

int parseLine(std::vector<float> &out, std::vector<int> &out2, std::vector<std::string> &out3, std::vector<DataBlock> &block, std::string &work); //return 1 for success, 0 for fail, -1 for comment
int parseLineMtl(std::vector<float> &out, std::vector<int> &out2, std::vector<std::string> &out3, std::vector<DataBlock> &block); //return 1 for success, 0 for fail, -1 for comment

bool loadObj(std::string& filePath, VisualObject &target);
bool loadMtl(std::string& filePath);
