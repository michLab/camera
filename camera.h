/**
  @file camera.h
  @brief A declarations used with Camera class
  @author Michal Labowski
  @date 22-12-2019
  @version 1.0
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/**
 * @namespace camera_ns
 * @brief a namespace to encapsulate Camera class environment
 */
namespace camera_ns {
    /**
     * @brief The CorrectionType enum to chose the camera
     * correction algorithm
     */
    enum class CorrectionType {
        remap,
        undistort
    };

    /**
     * @brief The ExceptionID enum
     */
    enum class ExceptionID {
        camera_open_failure,
        camera_wrong_id,
        images_count_to_small,
        camera_reading_failure,
        no_calibration_data,
        no_captured_frame,
        no_calibration_images,
        wrong_chessboard_dimensions,
        wrong_chessboard_square_dimension,
        wrong_calibration_file_name,
        empty_calibration_file_name,
        empty_frame
    };

    /**
     * @brief The ExceptionMessage struct to define the exception
     * thown by camera class
     */
    struct ExceptionMessage {
        std::string msg;
        ExceptionID id;
    };

    /**
     * @brief The Camera class
     */
    class Camera
    {
    public:
        Camera();
        ~Camera();
        bool find_chessboard_corners();
        bool set_chessboard_square_dimension(float arg_dim);
        bool set_chessboard_width(uint8_t arg_width);
        bool set_chessboard_height(uint8_t arg_height);
        bool set_chessboard_dimensions(uint8_t arg_width, uint8_t arg_height);
        bool set_video_source(int arg_camera_id);
        bool set_camera_calibration_results_file_name(std::string arg_file_name);
        bool set_calibrated(bool arg_calibrated);
        bool set_number_of_images_to_calibrate(uint8_t atg_num);
        bool set_default_camera_calibration_coefs();

        bool get_calibration_in_progress() const;
        bool get_calibrated() const;
        int get_camera_id() const;
        float get_chessboard_square_dimension() const;
        uint8_t get_chessboard_width() const;
        uint8_t get_chessboard_height() const;
        uint8_t get_number_of_images_to_calibrate() const;
        std::string get_camera_calibration_results_file_name() const;
        cv::Size get_chessboard_dimensions() const;
        cv::Mat get_frame_raw() const;

        void calibrate();
        void compensate_distortions(CorrectionType ct);
        void load_camera_calibration_data();
        void show_frame_raw() const;
        void show_frame_compensated() const;
        bool open();
        bool read();

    private:
        bool chessboard_found;
        bool calibration_in_progress;
        bool calibrated;
        bool remap_first_entry;
        int camera_id;
        float chessboard_square_dimension;
        uint8_t chessboard_width;
        uint8_t chessboard_height;
        uint8_t number_of_images_to_calibrate;
        uint8_t calibartion_image_number;
        std::string camera_calibration_file_name;
        std::vector<cv::Vec2f> chessboard_found_points;
        cv::Size chessboard_dimensions;
        cv::VideoCapture cam;
        cv::Mat captured_frame;
        cv::Mat cam_matrix;
        cv::Mat dist_coeffs;
        cv::Mat frame_compensated;
        cv::Mat remap_map1;
        cv::Mat remap_map2;
        cv::Size frame_size;

        void calibration_backend(std::vector<cv::Mat> calibration_images);
        void get_chessboard_corners(std::vector<cv::Mat> images,
                                    std::vector<std::vector<cv::Point2f>>& all_found_corners,
                                    bool show_results);
        void create_known_board_positions(std::vector<cv::Point3f> &corners);
        void put_calibration_info_on_image(cv::Mat& image);
        bool save_camera_calibration();
    };
}

#endif // CAMERA_H
