echo '=========================='
echo 'Compiling script [v]'
echo '=========================='
g++ send_an_image_server.cpp -o ./out/server.out
g++ send_an_image_client.cpp -o ./out/client.out

echo '=========================='
echo 'Running script [v]'
echo '=========================='
/bin/sh -ec './out/server.out 8700' &
/bin/sh -ec './out/client.out localhost 8700'

echo '=========================='
echo 'Waiting 5 seconds'
echo '=========================='
sleep 5

echo '=========================='
echo 'Removing artifects'
echo '=========================='
rm incoming-img.jpg
rm incoming-text.txt