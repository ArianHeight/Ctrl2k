#include "tests.h"

#include <iostream>

#include "Core/Monument/Monument.h"
#include "Core/GreatBath/FilePathProcessing.h"

void printFilePath(const gbt::FilePath& path)
{
#ifndef MUTE_FILEPATH_TEST_COUT
    std::cout << "file ext: " << path.fileExtView() << '\n';
    std::cout << "file name(no ext): " << path.fileNameNoExtView() << '\n';
    std::cout << "file name: " << path.fileNameView() << '\n';
    std::cout << "folder path: " << path.folderPathView() << '\n';
#endif // MUTE_FILEPATH_TEST_COUT
}

void testFilePathFunctionality(const gbt::FilePath& path, const std::string& folder, const std::string& nameNoExt, const std::string& ext, bool empty = false)
{
    std::cout << "===================================\nTesting path: " << path << '\n';
    printFilePath(path);
    assert(path.fileExt() == path.fileExtView());
    assert(path.fileName() == path.fileNameView());
    assert(path.fileNameNoExt() == path.fileNameNoExtView());
    assert(path.folderPath() == path.folderPathView());

    assert(path.isEmpty() == empty);
    assert(path.fileExtView() == ext);
    assert(path.fileNameNoExtView() == nameNoExt);
    assert(path.folderPathView() == folder);
    std::string name = nameNoExt.empty() || ext.empty() ? nameNoExt + ext : nameNoExt + "." + ext;
    assert(path.fileName() == name);
    std::cout << std::endl;
}

void initFilePath(const std::string& pathstring, gbt::FilePath& emptyPath)
{
    gbt::FilePath newPath = pathstring;
    emptyPath = std::move(newPath);
}

void runFilePathTests()
{
    gbt::FilePath emptyPath;
    testFilePathFunctionality(emptyPath, "", "", "", true);

    gbt::FilePath path1 = gbt::FilePath("C:\\Users\\blue\\Source\\Repos\\Ctrl2k\\Ctrl Playground\\Ctrl Playground.vcxproj.filters");
    testFilePathFunctionality(path1, "C:\\Users\\blue\\Source\\Repos\\Ctrl2k\\Ctrl Playground\\", "Ctrl Playground.vcxproj", "filters");

    gbt::FilePath path2 = gbt::FilePath("someFolder/childFolder\\blue.png");
    testFilePathFunctionality(path2, "someFolder/childFolder\\", "blue", "png");

    gbt::FilePath onlyFolder = gbt::FilePath("l1/l2\\");
    testFilePathFunctionality(onlyFolder, "l1/l2\\", "", "");

    gbt::FilePath onlyFile = gbt::FilePath("green.jpg");
    testFilePathFunctionality(onlyFile, "", "green", "jpg");

    gbt::FilePath noExt = gbt::FilePath("albert/einstein/tesla\\red");
    testFilePathFunctionality(noExt, "albert/einstein/tesla\\", "red", "");

    // manual test for filename
    gbt::FilePath noExtWeird = gbt::FilePath("no/ext\\weird.");
    assert(noExtWeird.fileName() == "weird.");
    assert(noExtWeird.fileNameNoExt() == "weird");

    gbt::FilePath noFolderWeird = gbt::FilePath("\\lol.csv");
    testFilePathFunctionality(noFolderWeird, "\\", "lol", "csv");

    gbt::FilePath noFolderWeird2 = gbt::FilePath("/sav.dat");
    testFilePathFunctionality(noFolderWeird2, "/", "sav", "dat");

    gbt::FilePath rvalPath;
    initFilePath("rval/test\\god.dog", rvalPath);
    testFilePathFunctionality(rvalPath, "rval/test\\", "god", "dog");

    gbt::FilePath newRvalPath(std::move(rvalPath));
    testFilePathFunctionality(newRvalPath, "rval/test\\", "god", "dog");

    gbt::FilePath lvalPath(newRvalPath);
    testFilePathFunctionality(lvalPath, "rval/test\\", "god", "dog");
    testFilePathFunctionality(newRvalPath, "rval/test\\", "god", "dog");

    std::string strTest = "string/test/yes/str.lng";

    gbt::FilePath strPath(strTest);
    testFilePathFunctionality(strPath, "string/test/yes/", "str", "lng");

    gbt::FilePath rvalStrPath(std::move(strTest));
    testFilePathFunctionality(rvalStrPath, "string/test/yes/", "str", "lng");

    gbt::FilePath revPath = "../..\\bluehills.xif";
    testFilePathFunctionality(revPath, "../..\\", "bluehills", "xif");

    gbt::FilePath revPathNoExt = "../..\\bluehills";
    testFilePathFunctionality(revPathNoExt, "../..\\", "bluehills", "");
}
