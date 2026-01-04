#include "tests.h"

#include <iostream>

#include "Core/Monument/Monument.h"
#include "Core/GreatBath/FilePathProcessing.h"

template <typename filepathtype>
void printFilePath(const filepathtype& path)
{
#ifndef MUTE_FILEPATH_TEST_COUT
    std::cout << "file ext: " << path.fileExtView() << '\n';
    std::cout << "file name(no ext): " << path.fileNameNoExtView() << '\n';
    std::cout << "file name: " << path.fileNameView() << '\n';
    std::cout << "folder path: " << path.folderPathView() << '\n';
#endif // MUTE_FILEPATH_TEST_COUT
}

template <typename filepathtype>
void testFilePathFunctionality(const filepathtype& path,
    const gbt::file_path_viewtype folder, const gbt::file_path_viewtype nameNoExt, const gbt::file_path_viewtype ext,
    bool empty = false, bool error = false)
{
    std::cout << "===================================\nTesting path: " << path << '\n';

    assert(path.isEmpty() == empty);
    assert(path.getFlags().hasError == error);
    if(path.getFlags().hasError)
        return;

    printFilePath(path);
    assert(path.fileExt() == path.fileExtView());
    assert(path.fileName() == path.fileNameView());
    assert(path.fileNameNoExt() == path.fileNameNoExtView());
    assert(path.folderPath() == path.folderPathView());
    
    assert(path.getFlags().isFolder == (nameNoExt.length() + ext.length() == 0));
    assert(path.getFlags().hasFolder == !folder.empty());
    assert(path.getFlags().hasExt == !ext.empty());
    assert(path.fileExtView() == ext);
    assert(path.fileNameNoExtView() == nameNoExt);
    assert(path.folderPathView() == folder);
    gbt::file_path_data name = nameNoExt;
    if(nameNoExt.empty() || ext.empty())
    {
        name += ext;
    }
    else
    {
        name += ".";
        name += ext;
    }
    assert(path.fileName() == name);
    std::cout << std::endl;
}

void runAllFilePathViewTests()
{
    gbt::FilePathView emptyPath;
    testFilePathFunctionality(emptyPath, "", "", "", true, true);

    gbt::FilePathView path1 = gbt::FilePathView("C:\\Users\\blue\\Source\\Repos\\Ctrl2k\\Ctrl Playground\\Ctrl Playground.vcxproj.filters");
    testFilePathFunctionality(path1, "C:\\Users\\blue\\Source\\Repos\\Ctrl2k\\Ctrl Playground\\", "Ctrl Playground.vcxproj", "filters");

    gbt::FilePathView path2 = gbt::FilePathView("someFolder/childFolder\\blue.png");
    testFilePathFunctionality(path2, "someFolder/childFolder\\", "blue", "png");

    gbt::FilePathView onlyFolder = gbt::FilePathView("l1/l2\\");
    testFilePathFunctionality(onlyFolder, "l1/l2\\", "", "");

    gbt::FilePathView onlyFile = gbt::FilePathView("green.jpg");
    testFilePathFunctionality(onlyFile, "", "green", "jpg");

    gbt::FilePathView noExt = gbt::FilePathView("albert/einstein/tesla\\red");
    testFilePathFunctionality(noExt, "albert/einstein/tesla\\", "red", "");

    // manual test for filename
    gbt::FilePathView noExtWeird = gbt::FilePathView("no/ext\\weird.");
    assert(noExtWeird.fileName() == "weird.");
    assert(noExtWeird.fileNameNoExt() == "weird");

    gbt::FilePathView noFolderWeird = gbt::FilePathView("\\lol.csv");
    testFilePathFunctionality(noFolderWeird, "\\", "lol", "csv");

    gbt::FilePathView noFolderWeird2 = gbt::FilePathView("/sav.dat");
    testFilePathFunctionality(noFolderWeird2, "/", "sav", "dat");

    gbt::file_path_data rvalData = "rval/test\\god.dog";
    gbt::FilePathView rvalPath = rvalData.view();
    testFilePathFunctionality(rvalPath, "rval/test\\", "god", "dog");

    gbt::FilePathView lvalPath(rvalPath);
    testFilePathFunctionality(lvalPath, "rval/test\\", "god", "dog");
    testFilePathFunctionality(rvalPath, "rval/test\\", "god", "dog");
    assert(lvalPath == rvalPath);
    assert(lvalPath == rvalData);
    assert(rvalData == lvalPath.pathView());

    gbt::FilePathView revPath = "../..\\bluehills.xif";
    testFilePathFunctionality(revPath, "../..\\", "bluehills", "xif");

    gbt::FilePathView revPathNoExt = "../..\\bluehills";
    testFilePathFunctionality(revPathNoExt, "../..\\", "bluehills", "");
}

void runAllFilePathTests()
{
    gbt::FilePath emptyPath;
    testFilePathFunctionality(emptyPath, "", "", "", true, true);

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

    gbt::file_path_data rvalData = "rval/test\\god.dog";
    gbt::FilePath rvalPath = rvalData.view();
    testFilePathFunctionality(rvalPath, "rval/test\\", "god", "dog");

    gbt::FilePath lvalPath(rvalPath);
    testFilePathFunctionality(lvalPath, "rval/test\\", "god", "dog");
    testFilePathFunctionality(rvalPath, "rval/test\\", "god", "dog");
    assert(lvalPath == rvalPath);
    assert(lvalPath == rvalData);
    assert(rvalData == lvalPath.pathView());

    std::string strTest = "string/test/yes/str.lng";

    gbt::FilePathView strTestView = strTest.c_str();
    gbt::FilePath strTestPath = strTestView;
    testFilePathFunctionality(strTestPath, "string/test/yes/", "str", "lng");
    testFilePathFunctionality(strTestPath.view(), "string/test/yes/", "str", "lng");

    gbt::FilePath revPath = "../..\\bluehills.xif";
    testFilePathFunctionality(revPath, "../..\\", "bluehills", "xif");

    gbt::FilePath revPathNoExt = "../..\\bluehills";
    testFilePathFunctionality(revPathNoExt, "../..\\", "bluehills", "");
}

void runFilePathTests()
{
    runAllFilePathViewTests();
    runAllFilePathTests();
}
