echo '=========================='
echo 'Compiling script [v]'
echo '=========================='
g++ -std=gnu++11 client.cpp -o client.out -l sqlite3
g++ -std=gnu++11 server.cpp -o server.out -l sqlite3
echo '=========================='
echo 'Running script [v]'
echo '=========================='
/bin/sh -ec './server.out' &
/bin/sh -ec './client.out'


