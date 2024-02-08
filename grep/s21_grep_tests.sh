#!/bin/bash

grepTests=()

#Base tests
grepTests+=("Far ./TestFiles/text_2.txt")
grepTests+=("\"\" ./TestFiles/text_2.txt")

#e tests
grepTests+=("-e ha -e M ./TestFiles/text_1.txt")
grepTests+=("-e ./TestFiles/text_1.txt ./TestFiles/text_2.txt")

#c tests
grepTests+=("-c f ./TestFiles/text_1.txt")
grepTests+=("-c -l L ./TestFiles/text_2.txt")
grepTests+=("\"\" ./TestFiles/text_1.txt -c")

#i tests
grepTests+=("-i d ./TestFiles/text_1.txt")
grepTests+=("-i -e d -e D ./TestFiles/text_2.txt")
grepTests+=("oVeR -i ./TestFiles/text_2.txt")

#v tests
grepTests+=("-v the ./TestFiles/text_2.txt")
grepTests+=("-v [^a] ./TestFiles/text_1.txt")

#l tests
grepTests+=("-l oo ./TestFiles/text_1.txt")
grepTests+=("-l Far ./TestFiles/text_1.txt ./TestFiles/text_2.txt")
grepTests+=("-l [^the] ./TestFiles/text_1.txt ./TestFiles/text_2.txt")

#n tests
grepTests+=("-n they ./TestFiles/text_2.txt")
grepTests+=("-n && ./TestFiles/text_1.txt")
grepTests+=("-n -e ee -e ee ./TestFiles/text_2.txt ./TestFiles/text_2.txt")

#h tests
grepTests+=("-h es ./TestFiles/text_1.txt ./TestFiles/text_2.txt")
grepTests+=("-h eig ./TestFiles/text_2.txt")
grepTests+=("-h -h u ./TestFiles/text_1.txt ./TestFiles/text_1.txt ./TestFiles/text_2.txt")

#s tests
grepTests+=("-s a ./")
grepTests+=("-s egs ./TestFiles/text_1.txt ./ text.txt ./TestFiles/text_2.txt")

#o tests
grepTests+=("-o on ./TestFiles/text_1.txt")
grepTests+=("-o -e ge -e lo ./TestFiles/text_2.txt ./TestFiles/text_1.txt")
grepTests+=("-o \"\" ./TestFiles/text_2.txt")

#f tests
grepTests+=("-f ./TestFiles/f_text.txt ./TestFiles/text_1.txt")
grepTests+=("-f ./TestFiles/f_text.txt -o ./TestFiles/text_1.txt")

#error check
errorTests=()

errorTests+=("./TestFiles/text_2.txt a")
errorTests+=("a ./")
errorTests+=("a -e ./TestFiles/text_2.txt")
errorTests+=("-v a ./")
errorTests+=("-s -g")
errorTests+=("-f ./ ./TestFiles/text_2.txt")

if [ -n "$1" ]
then
	if [[ $1 == "--memory-tests-Linux" ]]
	then	
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
		for i in ${!grepTests[@]}; do
			echo "Test --- $((i + 1))"
			echo "ENTRED VALUE: ${grepTests[$i]}"
			valgrind -q --leak-check=full ./s21_grep ${grepTests[$i]} >/dev/null
		done
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
	elif [[ $1 == "--memory-tests-Mac" ]]
	then
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
		for i in ${!grepTests[@]}; do
			echo "Test --- $((i + 1))"
			echo "ENTRED VALUE: ${grepTests[$i]}"
			leaks -atExit -- ./s21_grep ${grepTests[$i]}
		done
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
	elif [[ $1 == "--error-check" ]]
	then
		for i in ${!errorTests[@]}; do
			grep ${errorTests[$i]} >expected.txt 2>expected.txt
			./s21_grep ${errorTests[$i]} >received.txt
			echo
			echo "///////////////////////////"
			echo
			echo "Test --- $((i + 1))"
			echo
			echo "ENTRED VALUE: ${errorTests[$i]}"
			echo "Expected result is error: "
			echo
			echo "$(cat expected.txt)"
			echo
			echo "Received result: "
			echo
			echo "$(cat received.txt)"
			echo
			echo "///////////////////////////"
			echo
		done
		rm expected.txt received.txt
	else
		echo "s21_grep_tests.sh to run grep tests"
		echo "[OPTIONS]"
		echo "--memory-tests-Linux - run Valgrind tests"
		echo "--memory-tests-Mac - run Leak tests"
		echo "--error-check - run error check"
	fi
else
		echo "s21_grep test in progress: "
		echo
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
		echo
		for i in ${!grepTests[@]}; do
			grep ${grepTests[$i]} >expected.txt 2>expected.txt
			./s21_grep ${grepTests[$i]} >received.txt
			if diff expected.txt received.txt >/dev/null
			then
				echo "Test --- $((i + 1)): SUCCESS"
				
			else
				echo
				echo "///////////////////////////"
				echo
				echo "Test --- $((i + 1)): FAILED"
				echo 
				echo "ENTRED VALUE: ${grepTests[$i]}"
				echo "Expected result: "
				echo
				echo "$(cat expected.txt)"
				echo
				echo "Received result: "
				echo
				echo "$(cat received.txt)"
				echo
				echo "///////////////////////////"
				echo
			fi
		done
	echo
	echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
	rm expected.txt received.txt
fi

