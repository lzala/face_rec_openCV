# face_rec_openCV

Hints:

< build system >
 - make sure you have cmake and opencv correctly installed
 - cd build
 - cmake ..

< add new subject >
  - create new folder <SubjectName> in /data
  - add at least 10 images: grayscale + 100x100 resolution
  - ./facerec.sh - update (updates CVS file with the new subject)

< execute application >
  - cd util
  - ./facerec.sh -build -run
