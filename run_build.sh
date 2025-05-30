docker build . -f .devcontainer/Dockerfile -t temp
cd ..
docker run \
    -it --rm \
    -v ${PWD}/thirdparty:/workfiles/thirdparty \
    -v ${PWD}/fileutil:/workfiles/fileutil \
    -u vscode \
    -w /workfiles/fileutil \
    temp bash

cmake -S ./ -B ./build -G "Ninja"
cmake --install ./build --prefix ./export --config Release