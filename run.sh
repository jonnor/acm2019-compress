make
./compr dumps/mcf.core | grep 'Compression: ' | tr -d ':' | cut -d ' ' -f2 | awk -v p='mcf,cons,' '{print p $0}'
./compr dumps/x264.core | grep 'Compression: ' | tr -d ':' | cut -d ' ' -f2  | awk -v p='x264,cons,' '{print p $0}'
./compr dumps/bwaves.core | grep 'Compression: ' | tr -d ':' | cut -d ' ' -f2  | awk -v p='bwaves,cons,' '{print p $0}'
./compr dumps/xz.core | grep 'Compression: ' | tr -d ':' | cut -d ' ' -f2  | awk -v p='xz,cons,' '{print p $0}'
./compr dumps/namd.core | grep 'Compression: ' | tr -d ':' | cut -d ' ' -f2  | awk -v p='namd,cons,' '{print p $0}'
