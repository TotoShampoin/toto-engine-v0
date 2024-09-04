include(utils/CMakeUtils/files_and_folders.cmake)

function(Toto__target_copy_folder_absolute_paths
         TARGET
         INPUT_FOLDER
         OUTPUT_FOLDER
)
    # Copy each file
    file(GLOB_RECURSE FILES CONFIGURE_DEPENDS ${INPUT_FOLDER}/*)

    foreach(ABSOLUTE_FILE ${FILES})
        string(REPLACE ${INPUT_FOLDER} ${OUTPUT_FOLDER} OUTPUT_FILE ${ABSOLUTE_FILE})
        set(INPUT_FILE ${ABSOLUTE_FILE})
        Cool__target_copy_file_absolute_paths(${TARGET} ${INPUT_FILE} ${OUTPUT_FILE})
    endforeach()
endfunction()

