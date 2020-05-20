#!/bin/bash
INPUT_FILE="../fix_messages_5_0.json"
OUTPUT_FILE="../src/fix_db.h"

if [ ! -f "${INPUT_FILE}" ]
then
    echo "Can't open ${INPUT_FILE}"
    exit
fi

# Truncate the output file to start empty
echo "" > "${OUTPUT_FILE}"

# Output the C++ code for this file
echo "#pragma once" >> "${OUTPUT_FILE}"
echo "" >> "${OUTPUT_FILE}"
echo "#include <sstream>" >> "${OUTPUT_FILE}"
echo "#include <string>" >> "${OUTPUT_FILE}"
echo "" >> "${OUTPUT_FILE}"
echo "namespace rda" >> "${OUTPUT_FILE}"
echo "{" >> "${OUTPUT_FILE}"
echo "    class fix_db" >> "${OUTPUT_FILE}"
echo "    {" >> "${OUTPUT_FILE}"
echo "    public: " >> "${OUTPUT_FILE}"
echo "        static std::string get()" >> "${OUTPUT_FILE}"
echo "        {" >> "${OUTPUT_FILE}"
echo "            static std::stringstream ss;" >> "${OUTPUT_FILE}"
echo "            if (ss.str().empty())" >> "${OUTPUT_FILE}"
echo "            {" >> "${OUTPUT_FILE}"

echo "                ss << R\"RAW_DELIM("  >> "${OUTPUT_FILE}"

# Some C++ compilers limit the length of string literals.
# This will break up the json data into several chunks,
# and then put them into a singleton static stringstream.
COUNT=0
cat "${INPUT_FILE}" | while read line
do
    COUNT=$(( ${COUNT} + 1 ))
    if [[ $(( ${COUNT} % 500 )) == 0 ]]
    then
        COUNT=0;
        echo "                )RAW_DELIM\";" >> "${OUTPUT_FILE}"
        echo "                ss << R\"RAW_DELIM(" >> "${OUTPUT_FILE}"
    fi
        echo "${line}" >> "${OUTPUT_FILE}" >> "${OUTPUT_FILE}"
done

echo "                )RAW_DELIM\";" >> "${OUTPUT_FILE}"

echo "            }" >> "${OUTPUT_FILE}"
echo "            " >> "${OUTPUT_FILE}"
echo "            return ss.str();" >> "${OUTPUT_FILE}"
echo "        }" >> "${OUTPUT_FILE}"
echo "    };" >> "${OUTPUT_FILE}"
echo "}" >> "${OUTPUT_FILE}"
echo "" >> "${OUTPUT_FILE}"

# Convert line endings
dos2unix "${OUTPUT_FILE}"

