pushd tests
nmake /f nmakefile %1
IF "%1" == "" bin\neuron-test.exe
popd