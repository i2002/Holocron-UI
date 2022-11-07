cat "${INPUT_FILENAME}" | ./bin/"${EXECUTABLE_NAME}" &
sleep 10 && xdotool search --name "${APP_WINDOW}" key Alt+F4