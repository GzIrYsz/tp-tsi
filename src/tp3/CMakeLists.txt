add_executable(hough_transform hough_transform.c)
target_link_libraries(hough_transform PRIVATE nrc m)

add_executable(line_detection line_detection.c)
target_link_libraries(line_detection PRIVATE nrc m)