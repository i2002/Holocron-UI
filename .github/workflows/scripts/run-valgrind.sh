
cat "${INPUT_FILENAME}" | valgrind --leak-check=full --track-origins=yes --error-exitcode=0 ./bin/"${EXECUTABLE_NAME}" &
sleep 10 && xdotool search --name "${APP_WINDOW}" key Alt+F4
