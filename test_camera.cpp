#include <gtest/gtest.h>
#include "camera.h"

TEST(CameraTest, DefaultConstructor)
{
    camera_ns::Camera cam;
    uint8_t expected_height = 0;
    EXPECT_EQ(expected_height, cam.get_chessboard_height());

    uint8_t expected_width = 0;
    EXPECT_EQ(expected_width, cam.get_chessboard_width());

    EXPECT_EQ(expected_height, cam.get_chessboard_dimensions().height);
    EXPECT_EQ(expected_width, cam.get_chessboard_dimensions().width);

    float expected_square_size = 0.0f;
    EXPECT_FLOAT_EQ(expected_square_size, cam.get_chessboard_square_dimension());

    int expected_camera_id = -1;
    EXPECT_EQ(expected_camera_id, cam.get_camera_id());
}


TEST(CameraTest, CameraCalibrationExceptionWrongId)
{
    camera_ns::Camera cam;
    bool catch_exception = false;
    try {
        cam.calibrate();
    } catch (camera_ns::ExceptionMessage em) {
        //std::cout << em.msg << std::endl;
        catch_exception = true;
    }
    ASSERT_EQ(catch_exception, true);
}

//TEST(CameraTest, CameraCalibration)
//{
//    camera_ns::Camera cam;
//    cam.set_video_source(0);
//    cam.set_chessboard_dimensions(6,9);
//    cam.set_chessboard_square_dimension(0.0268f);
//    cam.set_number_of_images_to_calibrate(2);
//    bool catch_exception = false;
//    try {
//        cam.calibrate();
//    } catch (camera_ns::ExceptionMessage em) {
//        //std::cout << em.msg << std::endl;
//        catch_exception = true;
//    }
//    ASSERT_EQ(catch_exception, false);

//    bool expected_flag_state = true;
//    EXPECT_EQ(expected_flag_state, cam.get_calibrated());
//}

TEST(CameraTest, SetCameraCalibrationResultsFileName)
{
    camera_ns::Camera cam;
    std::string expected_file_name = "results.txt";
    cam.set_camera_calibration_results_file_name(expected_file_name);

    EXPECT_EQ(expected_file_name, cam.get_camera_calibration_results_file_name());
}

TEST(CameraTest, CameraCalibratedFlag)
{
    camera_ns::Camera cam;
    bool expected_flag_state = false;

    EXPECT_EQ(expected_flag_state, cam.get_calibrated());
}

TEST(CameraTest, LoadCameraDataFromFile)
{
    camera_ns::Camera cam;
    cam.set_camera_calibration_results_file_name("wyniki.tx");
    bool catch_exception = false;
    try {
        cam.load_camera_calibration_data();
    } catch (camera_ns::ExceptionMessage em) {
        //std::cout << em.msg << std::endl;
        catch_exception = true;
    }
    ASSERT_EQ(catch_exception, true);
}
