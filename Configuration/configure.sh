#!/bin/bash

#=============================================================================
#  Configure BuildRule.
#=============================================================================
printf 'configure BuildRule... '

./configure_BuildRule.py Core
./configure_BuildRule.py SupportGLUT
./configure_BuildRule.py SupportOpenCV
./configure_BuildRule.py SupportCUDA
./configure_BuildRule.py SupportPython
./configure_BuildRule.py SupportEGL
./configure_BuildRule.py SupportOSMesa

printf 'success\n'


#=============================================================================
#  Configure KVS_HEADER_LIST.
#=============================================================================
printf 'configure KVS_HEADER_LIST... '

for FILENAME in $( find .. -name 'KVS_HEADER_LIST' ); do
    sort -u -o ${FILENAME} ${FILENAME}
done

printf 'success\n'


#=============================================================================
#  Configure header list.
#=============================================================================
printf 'configure header list.. '

#rm -f $( find ../Source/kvs -type f -not -path '*/.svn/*' )
rm -f $( find ../Source/kvs -type f -not -path '*/.git/*' )

./configure_header_list.py Core             kvs
./configure_header_list.py SupportGLUT      kvs/glut
./configure_header_list.py SupportOpenCV    kvs/opencv
./configure_header_list.py SupportQt        kvs/qt
./configure_header_list.py SupportCUDA      kvs/cuda
./configure_header_list.py SupportPython    kvs/python
./configure_header_list.py SupportEGL       kvs/egl
./configure_header_list.py SupportOSMesa    kvs/osmesa

printf 'success\n'


#=============================================================================
#  Configure permission.
#=============================================================================
printf 'configure permission... '

#chmod 644 $( grep -I -l '' $( find .. -type f -not -path '*/.svn/*' ) )
chmod 644 $( grep -I -l '' $( find .. -type f -not -path '*/.git/*' ) )
chmod 755 $( find .. -type d )
chmod 755 $( find .. -name '*.sh' -o -name '*.py')

printf 'success\n'
