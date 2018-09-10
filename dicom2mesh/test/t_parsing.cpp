#include <gtest/gtest.h>
#include "dicom2mesh.h"

TEST(ArgumentParser, InputPathAndDefault)
{
    int nInput = 2;
    const char* input[2] = { "-i", "inputDir"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.pathToInputAvailable);
    ASSERT_STREQ(parsedInput.pathToInputData.c_str(),"inputDir");

    ASSERT_FALSE(parsedInput.enableMeshReduction);
    ASSERT_FALSE(parsedInput.enableCrop);
    ASSERT_FALSE(parsedInput.enablePolygonLimitation);
    ASSERT_FALSE(parsedInput.enableSmoothing);
    ASSERT_FALSE(parsedInput.enableObjectFiltering);
    ASSERT_FALSE(parsedInput.enableOriginToCenterOfMass);

    ASSERT_FALSE(parsedInput.pathToOutputAvailable);
}

TEST(ArgumentParser, OutputPath)
{
    int nInput = 4;
    const char *input[4] = {"-i", "inputDir", "-o", "output.obj"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.pathToOutputAvailable);
    ASSERT_STREQ(parsedInput.outputFilePath.c_str(), "output.obj");
}

TEST(ArgumentParser, Threshold)
{
    int nInput = 4;
    const char *input[4] = {"-i", "inputDir", "-t", "405"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_EQ(parsedInput.isoValue, 405);
}

TEST(ArgumentParser, ThresholdNegative)
{
    int nInput = 4;
    const char *input[4] = {"-i", "inputDir", "-t", "-24"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_EQ(parsedInput.isoValue, -24);
}

TEST(ArgumentParser, Reduction)
{
    int nInput = 4;
    const char *input[4] = {"-i", "inputDir", "-r", "0.43"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.enableMeshReduction);
    ASSERT_FLOAT_EQ(parsedInput.reductionRate, 0.43);
}

TEST(ArgumentParser, SmoothingAndFilter)
{
    int nInput = 5;
    const char *input[5] = {"-i", "inputDir", "-s", "-e", "0.1234"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.enableSmoothing);

    ASSERT_TRUE(parsedInput.enableObjectFiltering);
    ASSERT_FLOAT_EQ(parsedInput.objectSizeRatio, 0.1234);
}

TEST(ArgumentParser, CenterAndCrop)
{
    int nInput = 4;
    const char *input[4] = {"-i", "inputDir", "-c", "-z"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.enableOriginToCenterOfMass);
    ASSERT_TRUE(parsedInput.enableCrop);
}


TEST(ArgumentParser, Visualization)
{
    int nInput = 3;
    const char *input[3] = {"-i", "inputDir", "-v"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.doVisualize);
    ASSERT_FALSE(parsedInput.showAsVolume);
}


TEST(ArgumentParser, VolVisualization)
{
    int nInput = 15;
    const char *input[15] = {"-i", "inputDir", "-vo", "( ", "1,", "2  ,", " 3 ,", "4", " ,-5)", "(6 , ", "7,", "+8,", "9 ,", "10 ", ")" };

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.doVisualize);
    ASSERT_TRUE(parsedInput.showAsVolume);

    ASSERT_EQ(2,parsedInput.volumenRenderingColoring.size());

    ASSERT_EQ(1, parsedInput.volumenRenderingColoring.at(0).red);
    ASSERT_EQ(2, parsedInput.volumenRenderingColoring.at(0).green);
    ASSERT_EQ(3, parsedInput.volumenRenderingColoring.at(0).blue);
    ASSERT_EQ(4, parsedInput.volumenRenderingColoring.at(0).alpha);
    ASSERT_EQ(-5, parsedInput.volumenRenderingColoring.at(0).voxelValue);

    ASSERT_EQ(6, parsedInput.volumenRenderingColoring.at(1).red);
    ASSERT_EQ(7, parsedInput.volumenRenderingColoring.at(1).green);
    ASSERT_EQ(8, parsedInput.volumenRenderingColoring.at(1).blue);
    ASSERT_EQ(9, parsedInput.volumenRenderingColoring.at(1).alpha);
    ASSERT_EQ(10, parsedInput.volumenRenderingColoring.at(1).voxelValue);
}

TEST(ArgumentParser, VolVisualizationInvalidColor)
{
    int nInput = 9;
    const char *input[9] = {"-i", "inputDir", "-vo", "( ", "257,", "2  ,", " 3 ,", "4", " ,-5)"};

    Dicom2MeshParameters parsedInput;
    ASSERT_FALSE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));
}

TEST(ArgumentParser, ManyParamsEnabled)
{
    int nInput = 12;
    const char *input[12] = {"-i", "inputDir", "-c", "-z", "-v", "-s", "-e", "0.1234", "-r", "0.43","-o", "output.obj"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.pathToInputAvailable);
    ASSERT_STREQ(parsedInput.pathToInputData.c_str(),"inputDir");

    ASSERT_TRUE(parsedInput.doVisualize);
    ASSERT_FALSE(parsedInput.showAsVolume);

    ASSERT_TRUE(parsedInput.enableOriginToCenterOfMass);
    ASSERT_TRUE(parsedInput.enableCrop);

    ASSERT_TRUE(parsedInput.enableSmoothing);

    ASSERT_TRUE(parsedInput.enableObjectFiltering);
    ASSERT_FLOAT_EQ(parsedInput.objectSizeRatio, 0.1234);

    ASSERT_TRUE(parsedInput.enableMeshReduction);
    ASSERT_FLOAT_EQ(parsedInput.reductionRate, 0.43);

    ASSERT_TRUE(parsedInput.pathToOutputAvailable);
    ASSERT_STREQ(parsedInput.outputFilePath.c_str(), "output.obj");
}

TEST(ArgumentParser, ManyParamsDisabled)
{
    int nInput = 2;
    const char *input[2] = {"-i", "inputDir"};

    Dicom2MeshParameters parsedInput;
    ASSERT_TRUE(Dicom2Mesh::parseCmdLineParameters(nInput, input, parsedInput));

    ASSERT_TRUE(parsedInput.pathToInputAvailable);
    ASSERT_STREQ(parsedInput.pathToInputData.c_str(),"inputDir");

    ASSERT_FALSE(parsedInput.doVisualize);
    ASSERT_FALSE(parsedInput.showAsVolume);

    ASSERT_FALSE(parsedInput.enableOriginToCenterOfMass);
    ASSERT_FALSE(parsedInput.enableCrop);

    ASSERT_FALSE(parsedInput.enableSmoothing);

    ASSERT_FALSE(parsedInput.enableObjectFiltering);

    ASSERT_FALSE(parsedInput.enableMeshReduction);

    ASSERT_FALSE(parsedInput.pathToOutputAvailable);
}

