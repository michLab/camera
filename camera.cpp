/**
  @file camera.cpp
  @brief A definitions used with Camera class
  @author Michal Labowski
  @date 22-12-2019
  @version 1.0
 */

#include <iostream>
#include <fstream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "camera.h"


using namespace camera_ns;

/**
 * @brief: A default constructor
 */
Camera::Camera()
{
    set_chessboard_dimensions(0, 0);
    set_chessboard_square_dimension(0.0f);
    set_video_source(-1);
    set_camera_calibration_results_file_name("cam_calib_results.txt");
    set_calibrated(false);
    set_number_of_images_to_calibrate(0);
    set_default_camera_calibration_coefs();
    calibration_in_progress = false;
    calibartion_image_number = 0;
    remap_first_entry = true;
}

/**
 * @brief: A default destructor
 */
Camera::~Camera()
{

}

/**
 * @brief: Sets the size of single squre of chessboard
 * @param: arg_dim A side size of single square of chessboard [m]
 * @return: true
 */
bool Camera::set_chessboard_square_dimension(float arg_dim)
{
    chessboard_square_dimension = arg_dim;
    return true;
}

/**
 * @brief: Sets the chessboard width (a card should be placed horizontally)
 * @param: arg_width A width of chessborad (number of square corners)
 * @return: true
 */
bool Camera::set_chessboard_width(uint8_t arg_width)
{
    chessboard_width = arg_width;
    return true;
}

/**
 * @brief: Sets the chessboard height (a card should be placed horizontally)
 * @param: arg_width A height of chessborad (number of square corners)
 * @return: true
 */
bool Camera::set_chessboard_height(uint8_t arg_height)
{
    chessboard_height = arg_height;
    return true;
}

/**
 * @brief: Sets the chessboard width and height (a card should be placed horizontally)
 * @param: arg_width A width of chessborad (number of square corners)
 * @param: arg_height A height of chessborad (number of square corners)
 * @return: true
 */
bool Camera::set_chessboard_dimensions(uint8_t arg_height, uint8_t arg_width)
{
    set_chessboard_width(arg_width);
    set_chessboard_height(arg_height);
    chessboard_dimensions = cv::Size(chessboard_height, chessboard_width);
    return true;
}

/**
 * @brief: Sets the camera ID
 * @param: arg_camera_id The camera ID
 * @return: true
 */
bool Camera::set_video_source(int arg_camera_id)
{
    camera_id = arg_camera_id;
    return true;
}

/**
 * @brief: Sets camera calibration file name
 * @param: arg_file_name The calibrationr esults file name
 * @return: true
 */
bool Camera::set_camera_calibration_results_file_name(std::string arg_file_name)
{
    camera_calibration_file_name = arg_file_name;
    return true;
}

/**
 * @brief: Sets camera calibrated flag
 * @param: arg_calibrated The new state of that flag
 * @return: true
 */
bool Camera::set_calibrated(bool arg_calibrated)
{
    calibrated = arg_calibrated;
    return true;
}

/**
 * @brief: Sets number of images needed to perform calibration
 * @param: arg_num the number of images needed to perform calibration
 * @return: true
 */
bool Camera::set_number_of_images_to_calibrate(uint8_t arg_num)
{
    number_of_images_to_calibrate = arg_num;
    return true;
}

/**
 * @brief: Sets default camera matrix and dist coefficients values
 * @return: true
 */
bool Camera::set_default_camera_calibration_coefs()
{
    cam_matrix = cv::Mat::eye(3,3, CV_64F);
    dist_coeffs = cv::Mat::zeros(3,3, CV_64F);
    return true;
}

/**
 * @brief: Returns a width (card placed horizontally) of chessboard
 * @return: A width of chessboard (card placed horizontally)
 */
uint8_t Camera::get_chessboard_width() const
{
    return chessboard_width;
}

/**
 * @brief: Returns a height (card placed horizontally) of chessboard
 * @return: A height of chessboard (card placed horizontally)
 */
uint8_t Camera::get_chessboard_height() const
{
    return chessboard_height;
}

/**
 * @brief: Returns number of images needed to perform calibration
 * @return: The number of images needed to perform calibration
 */
uint8_t Camera::get_number_of_images_to_calibrate() const
{
    return number_of_images_to_calibrate;
}

/**
 * @brief: Returns a height and width (card placed horizontally) of chessboard
 * @return: A height and width of chessboard (card placed horizontally)
 */
cv::Size Camera::get_chessboard_dimensions() const
{
    return chessboard_dimensions;
}


/**
 * @brief: Returns a raw captured frame
 * @return: a cv::Mat frame
 */
cv::Mat Camera::get_frame_raw() const
{
    return captured_frame;
}

/**
 * @brief: Returns a calibrated frame
 * @return: a cv::Mat frame
 */
cv::Mat Camera::get_frame_calibrated() const
{
    return frame_compensated;
}

/**
 * @brief: Returns a pointer to raw captured frame
 * @return: a cv::Mat frame pointer
 */
cv::Mat *Camera::get_pointer_to_frame_raw()
{
    return &captured_frame;
}

/**
 * @brief: Returns a pointer to compensated frame
 * @return: a cv::Mat frame pointer
 */
cv::Mat *Camera::get_pointer_to_frame_calibrated()
{
    return &frame_compensated;
}

/**
 * @brief: Returns a side size of single square of chessboard
 * @return: A side size of single sqare from chessboard
 */
float Camera::get_chessboard_square_dimension() const
{
    return chessboard_square_dimension;
}

/**
 * @brief: Returns a camera ID
 * @return: A camera id
 */
int Camera::get_camera_id() const
{
    return camera_id;
}

/**
 * @brief: Returns a camera calibration file name
 * @return: Camera calibration file name
 */
std::string Camera::get_camera_calibration_results_file_name() const
{
    return camera_calibration_file_name;
}

/**
 * @brief: Check if camera calibration is in progress
 * @return: Camera calibration progress status
 */
bool Camera::get_calibration_in_progress() const
{
    return calibration_in_progress;
}

/**
 * @brief: Check if camera calibrated flag is set
 * @return: calibrated flag
 */
bool Camera::get_calibrated() const
{
    return calibrated;
}

/**
 * @brief: Opens camera for set camera id
 * @return: return true when camera was succesfully opened
 */
bool Camera::open()
{
    bool res = cam.open(camera_id);
    if(res == false) {
        ExceptionMessage em;
        em.msg = "Cannot open camera with id: " + std::to_string(camera_id);
        em.id = ExceptionID::camera_wrong_id;
        throw em;
    }
    return true;
}

/**
 * @brief: Calibarte camera distortions
 */
void Camera::calibrate()
{
    if(cam.isOpened() == false) {
        open();
    }
    if (camera_id == -1) {
        ExceptionMessage em;
        em.msg = "Cannot calibrate camera with id: " + std::to_string(camera_id);
        em.id = ExceptionID::camera_wrong_id;
        throw em;
    }
    if (number_of_images_to_calibrate == 0) {
        ExceptionMessage em;
        em.msg = "Number of images to calibrate should be greater than 0";
        em.id = ExceptionID::no_calibration_images;
        throw em;
    }

    cv::Mat frame_with_chessboard;
    std::vector<cv::Mat> saved_images;
    cv::Mat camera_matrix = cv::Mat::eye(3,3, CV_64F);

    cv::namedWindow("Raw", CV_WINDOW_AUTOSIZE);
    calibration_in_progress = true;
    calibrated = false;
    calibartion_image_number = 0;

    while (calibration_in_progress) {
        if (cam.read(captured_frame) == false) {
            ExceptionMessage em;
            em.msg = "Cannot read frame from camera with id: " + std::to_string(camera_id);
            em.id = ExceptionID::camera_reading_failure;
            calibration_in_progress = false;
            throw em;
        }

        chessboard_found = findChessboardCorners(captured_frame, chessboard_dimensions,
                                                 chessboard_found_points,
                                                 CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
        captured_frame.copyTo(frame_with_chessboard);
        drawChessboardCorners(frame_with_chessboard, chessboard_dimensions,
                              chessboard_found_points, chessboard_found);
        cv::Mat frame_to_show;
        if(chessboard_found) {
            frame_with_chessboard.copyTo(frame_to_show);

        } else {
            captured_frame.copyTo(frame_to_show);
        }
        put_calibration_info_on_image(frame_to_show);
        imshow("Raw", frame_to_show);
        char character = static_cast<char>(cv::waitKey(10));
        switch(character) {
            case ' ':
                /// saving image:
                if(chessboard_found)
                {
                    cv::Mat temp;
                    captured_frame.copyTo(temp);
                    saved_images.push_back(temp);
                    ++calibartion_image_number;
                }
                break;
            case 27:
                /// exit:
                calibration_in_progress = false;
                break;
        }
        /// start calibration (enter key)
        if (calibartion_image_number >= number_of_images_to_calibrate) {
            calibration_backend(saved_images);
            save_camera_calibration();
            calibrated = true;
            calibration_in_progress = false;
            break;
        }
    }
}

/**
 * @brief: Read data from camera camera distortions
 * @return: true when reading was successful
 */
bool Camera::read()
{
    if (camera_id == -1) {
        ExceptionMessage em;
        em.msg = "Cannot read from camera with id: " + std::to_string(camera_id);
        em.id = ExceptionID::camera_wrong_id;
        throw em;
    }
    if (cam.isOpened() == false) {
        open();
    }
    bool res = cam.read(captured_frame);
    frame_size = captured_frame.size();
    return res;
}

/**
 * @brief: Compensate distortions using selected algorithm
 */
void Camera::compensate_distortions(CorrectionType ct)
{
    if (calibrated == false) {
        ExceptionMessage em;
        em.msg = "Cannot compensate image without calibration data";
        em.id = ExceptionID::no_calibration_data;
        throw em;
    }
    if (captured_frame.empty() == true) {
        ExceptionMessage em;
        em.msg = "Cannot compensate image without captured frame";
        em.id = ExceptionID::empty_frame;
        throw em;
    }

    switch(ct){
        case CorrectionType::remap:
            if (remap_first_entry) {
                remap_first_entry = false;
                initUndistortRectifyMap(cam_matrix, dist_coeffs, cv::Mat(),
                                        getOptimalNewCameraMatrix(cam_matrix, dist_coeffs,
                                                                  frame_size, 1, frame_size, 0),
                                        frame_size, CV_16SC2, remap_map1, remap_map2);
            }
            remap(captured_frame, frame_compensated, remap_map1, remap_map2, cv::INTER_LINEAR);
            break;
        case CorrectionType::undistort:
            undistort(captured_frame, frame_compensated,
                      getOptimalNewCameraMatrix(cam_matrix, dist_coeffs,
                      frame_size, 1, frame_size, nullptr), dist_coeffs);
            break;
        default:
            frame_compensated = captured_frame;
            break;
    }
}

/**
 * @brief: A calibration backend function using openCV
 * @param calibration_images a vector of chessboard images
 */
void Camera::calibration_backend(std::vector<cv::Mat> calibration_images)
{
    std::vector<std::vector<cv::Point2f>> chessboard_image_space_points;
    if (calibration_images.size() == 0) {
        ExceptionMessage em;
        em.msg = "Cannot calibrate camera with no calibration images";
        em.id = ExceptionID::no_calibration_images;
        calibration_in_progress = false;
        throw em;
    }
    get_chessboard_corners(calibration_images,
                           chessboard_image_space_points, false);

    std::vector<std::vector<cv::Point3f>> world_space_corner_points(1);

    if (chessboard_dimensions.width == 0 or chessboard_dimensions.height == 0) {
        ExceptionMessage em;
        em.msg = "Cannot calibrate camera with chessboard 0 dimension";
        em.id = ExceptionID::wrong_chessboard_dimensions;
        calibration_in_progress = false;
        throw em;
    }
    create_known_board_positions(world_space_corner_points[0]);
    world_space_corner_points.resize(chessboard_image_space_points.size(),
                                     world_space_corner_points[0]);

    std::vector<cv::Mat> r_vectors, t_vectors;
    dist_coeffs = cv::Mat::zeros(8, 1, CV_64F);

    calibrateCamera(world_space_corner_points, chessboard_image_space_points,
                    chessboard_dimensions, cam_matrix, dist_coeffs, r_vectors, t_vectors);
}

/**
 * @brief: Finds chessboard corners
 * @param images a vector of chessboard images
 * @param all_found_corners a vector for found corners
 * @param a flag to show results
 */
void Camera::get_chessboard_corners(std::vector<cv::Mat> images,
                                    std::vector<std::vector<cv::Point2f>> &all_found_corners,
                                    bool show_results)
{
    for(std::vector<cv::Mat>::iterator iter = images.begin(); iter != images.end(); iter++)
    {
        std::vector<cv::Point2f> point_buf;
        bool found = findChessboardCorners(*iter, cv::Size(6,9), point_buf,
                                           CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

        if (found){
            all_found_corners.push_back(point_buf);
        }
        if(show_results){
            drawChessboardCorners(*iter, cv::Size(6,9), point_buf, found);
            imshow("Looking for corners", *iter);
            cv::waitKey(0);
        }
    }
}

/**
 * @brief: Creates known board positions
 * @param corners a 3Dvector reference for found corners
 */
void Camera::create_known_board_positions(std::vector<cv::Point3f> &corners)
{
    if (chessboard_dimensions.width == 0 or chessboard_dimensions.height == 0) {
        ExceptionMessage em;
        em.msg = "Cannot calibrate camera with chessboard 0 dimension";
        em.id = ExceptionID::wrong_chessboard_dimensions;
        calibration_in_progress = false;
        throw em;
    }
    if (chessboard_square_dimension == 0.0f) {
        ExceptionMessage em;
        em.msg = "Cannot calibrate camera when chesboard square size equals 0";
        em.id = ExceptionID::wrong_chessboard_square_dimension;
        calibration_in_progress = false;
        throw em;
    }
    for(int i = 0; i < chessboard_dimensions.height; i++) {
        for (int j = 0; j < chessboard_dimensions.width; j++) {
            corners.push_back(cv::Point3f(j * chessboard_square_dimension,
                                          i * chessboard_square_dimension, 0.0f));
        }
    }
}

/**
 * @brief: Save camera calibration results to file
 */
bool Camera::save_camera_calibration()
{
    if (camera_calibration_file_name == " ") {
        ExceptionMessage em;
        em.msg = "File name cannot be ' '";
        em.id = ExceptionID::empty_calibration_file_name;
        calibration_in_progress = false;
        throw em;
    }
    std::ofstream out_stream(camera_calibration_file_name);
    if (out_stream){
        uint16_t rows = static_cast<uint16_t>(cam_matrix.rows);
        uint16_t columns = static_cast<uint16_t>(cam_matrix.cols);
        out_stream << rows << std::endl;
        out_stream << columns << std::endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double value = cam_matrix.at<double>(r, c);
                out_stream << value << std::endl;
            }
        }

        rows = static_cast<uint16_t>(dist_coeffs.rows);
        columns = static_cast<uint16_t>(dist_coeffs.cols);
        out_stream << rows << std::endl;
        out_stream << columns << std::endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double value = dist_coeffs.at<double>(r, c);
                out_stream << value << std::endl;
            }
        }
        out_stream.close();
        return true;
    }
    return false;
}

/**
 * @brief: Load camera calibration results from file
 */
void Camera::load_camera_calibration_data()
{
    if (camera_calibration_file_name == " ") {
        ExceptionMessage em;
        em.msg = "File name cannot be ' '";
        em.id = ExceptionID::empty_calibration_file_name;
        calibration_in_progress = false;
        throw em;
    }
    std::ifstream in_stream;
    in_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      in_stream.open(camera_calibration_file_name);
    }
    catch (std::ifstream::failure e) {
        ExceptionMessage em;
        em.msg = "Exception opening the file named: " + camera_calibration_file_name;
        em.id = ExceptionID::wrong_calibration_file_name;
        calibration_in_progress = false;
        throw em;
    }

    if (in_stream) {
        uint16_t rows;
        uint16_t columns;
        in_stream >> rows;
        in_stream >> columns;
        cam_matrix = cv::Mat(cv::Size(columns, rows), CV_64F);
        for(int r = 0; r < rows; ++r) {
            for(int c = 0; c < columns; ++c) {
                double read = 0.0;
                in_stream >> read;
                cam_matrix.at<double>(r,c) = read;
            }
        }
        /// dist_coefficients
        in_stream >> rows;
        in_stream >> columns;
        dist_coeffs = cv::Mat::zeros(rows, columns, CV_64F);
        for(int r = 0; r < rows; ++r) {
            for(int c = 0; c < columns; ++c) {
                double read = 0.0;
                in_stream >> read;
                dist_coeffs.at<double>(r,c) = read;
            }
        }
        in_stream.close();
        set_calibrated(true);
    }
}

/**
 * @brief: Show window with raw camera image
 */
void Camera::show_frame_raw() const
{
    if (captured_frame.empty() == true) {
        ExceptionMessage em;
        em.msg = "Cannot show empty frame";
        em.id = ExceptionID::empty_frame;
        throw em;
    }
    cv::imshow("Raw", captured_frame);
}

/**
 * @brief: Show window with compensated camera image
 */
void Camera::show_frame_compensated() const
{
    if (frame_compensated.empty() == true) {
        ExceptionMessage em;
        em.msg = "Cannot show empty frame";
        em.id = ExceptionID::empty_frame;
        throw em;
    }
    cv::imshow("Compensated", frame_compensated);
}

/**
 * @brief: Put calbration indo into camera window
 */
void Camera::put_calibration_info_on_image(cv::Mat &image)
{
    std::string tmp_str = "Camera Calibration";
    putText(image, tmp_str, cvPoint(30,30),
        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,255,0), 1, CV_AA);
    tmp_str = "Image: " + std::to_string(calibartion_image_number) + "/"
            + std::to_string(number_of_images_to_calibrate);
    putText(image, tmp_str, cvPoint(30,50),
        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,255,0), 1, CV_AA);
}

