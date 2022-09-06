echo '=========================='
echo 'Compiling script [v]'
echo '=========================='
g++ ./file_server.cpp -o ./client.out
g++ ./file_client.cpp -o ./server.out

echo '=========================='
echo 'Running script [v]'
echo '=========================='
/bin/sh -ec './client.out' &
/bin/sh -ec './server.out'

