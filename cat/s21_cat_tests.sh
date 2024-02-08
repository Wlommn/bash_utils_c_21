#!/bin/bash

catTests=()

#Base tests
catTests+=("./TestFiles/text_1.txt")
catTests+=("./TestFiles/text_2.txt ./TestFiles/text_1.txt")

#b tests
catTests+=("-b ./TestFiles/text_1.txt")
catTests+=("-b ./TestFiles/text_2.txt ./TestFiles/text_1.txt")
catTests+=("-b ./TestFiles/empty_test.txt")

#e tests
catTests+=("-e ./TestFiles/text_1.txt")
catTests+=("-e ./TestFiles/text_2.txt ./TestFiles/text_1.txt")
catTests+=("-e ./TestFiles/empty_test.txt")

#n tests
catTests+=("-n ./TestFiles/text_1.txt")
catTests+=("-n ./TestFiles/text_2.txt ./TestFiles/text_1.txt")
catTests+=("-n ./TestFiles/empty_test.txt")

#s tests
catTests+=("-s ./TestFiles/text_2.txt")
catTests+=("-s ./TestFiles/empty_test.txt")

#t tests
catTests+=("-t ./TestFiles/text_1.txt")
catTests+=("-t ./TestFiles/text_2.txt ./TestFiles/text_1.txt")
catTests+=("-t ./TestFiles/empty_test.txt")

#v tests
catTests+=("-v ./TestFiles/v_test.txt")

#error check
errorTests=()

errorTests+=("./")
errorTests+=("./ text")

if [ -n "$1" ]
then
	if [[ $1 == "--memory-tests-Linux" ]]
	then	
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
		for i in ${!catTests[@]}; do
			echo "Test --- $((i + 1))"
			echo "ENTRED VALUE: ${catTests[$i]}"
			valgrind -q --leak-check=full ./s21_cat ${catTests[$i]} >/dev/null
		done
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
	elif [[ $1 == "--memory-tests-Mac" ]]
	then
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
		for i in ${!catTests[@]}; do
			echo "Test --- $((i + 1))"
			echo "ENTRED VALUE: ${catTests[$i]}"
			leaks -atExit -- ./s21_cat ${catTests[$i]}
		done
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
	elif [[ $1 == "--error-check" ]]
	then
		for i in ${!errorTests[@]}; do
			cat ${errorTests[$i]} >expected.txt 2>expected.txt
			./s21_cat ${errorTests[$i]} >received.txt
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
		echo "s21_cat_tests.sh to run cat tests"
		echo "[OPTIONS]"
		echo "--memory-tests-Linux - run Valgrind tests"
		echo "--memory-tests-Mac - run Leak tests"
		echo "--error-check - run error check"
	fi
else
		echo "s21_cat test in progress: "
		echo
		echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
		echo
		for i in ${!catTests[@]}; do
			cat ${catTests[$i]} >expected.txt 2>expected.txt
			./s21_cat ${catTests[$i]} >received.txt
			if diff expected.txt received.txt >/dev/null
			then
				echo "Test --- $((i + 1)): SUCCESS"
			else
				echo
				echo "///////////////////////////"
				echo
				echo "Test --- $((i + 1)): FAILED"
				echo 
				echo "ENTRED VALUE: ${catTests[$i]}"
				echo "Expected result: "
				echo "---------------------------"
				echo "$(cat expected.txt)"
				echo "---------------------------"
				echo "Received result: "
				echo "---------------------------"
				echo "$(cat received.txt)"
				echo "---------------------------"
				echo "///////////////////////////"
				echo
			fi
		done
	echo
	echo "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-"
	rm expected.txt received.txt
fi

