set(FTP_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(FTP_SOURCES 
  "${FTP_DIR}/main.cpp"
  "${FTP_DIR}/ControlConnection.cpp"
)

add_executable(ftp-server ${FTP_SOURCES})