cd ../g100/httpserver/prj_linux
make

cd ../../../g200

cd g200/prj_linux_v5
make
cd ../prj_linux_vsv1r4b2
make
cd ../prj_linux_vsv1r4b3
make

cd ../..
cd g200ms/prj_linux_v5
make
cd ../prj_linux_vsv1r4b2
make
cd ../prj_linux_vsv1r4b3
make

cd ../..
cd g200update
chmod +x maketarexe.sh
./maketarexe.sh