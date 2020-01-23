#!/bin/bash
FAIL=0

# Test the conceal program.
testConceal() {
  TESTNO=$1
  IMGFILE=$2
  BITCOUNT=$3

  rm -f output.ppm stdout.txt stderr.txt

  # Hide a message file in an image.
  echo "Conceal test $TESTNO: ./conceal message-$TESTNO.txt image-$IMGFILE.ppm output.ppm $BITCOUNT"
  ./conceal message-$TESTNO.txt image-$IMGFILE.ppm output.ppm $BITCOUNT > stdout.txt 2> stderr.txt
  STATUS=$?

  # Make sure the output file looks right.
  if [ -f concealed-$TESTNO.ppm ]; then
      if ! diff -q concealed-$TESTNO.ppm output.ppm >/dev/null 2>&1
      then
	  echo "**** Conceal test $TESTNO FAILED - output didn't match concealed-$TESTNO.ppm"
	  FAIL=1
	  return 1
      fi
  fi

  # For the program shouldn't print any output to standard output
  if [ -s stdout.txt ] ;then
      echo "**** Conceal test $TESTNO FAILED - program shouldn't print any output to stdout."
      FAIL=1
      return 1
  fi
      
  # If an error output file exists.
  if [ -f estderr-$TESTNO.txt ] ; then
      # Make sure we get the right error message and exit unsuccessfully.
      if [ $STATUS != 1 ]; then
	  echo "**** Conceal test $TESTNO FAILED - program should exit unsuccessfully for this test."
	  FAIL=1
	  return 1
      fi
      
      if ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      then
	  echo "**** Conceal test $TESTNO FAILED - program didn't print the right error message."
	  FAIL=1
	  return 1
      fi
  else
      # On a successful test, there should be no error output and the program
      # should exit successfully
      if [ $STATUS != 0 ]; then
	  echo "**** Conceal test $TESTNO FAILED - program should exit successfully for this test."
	  FAIL=1
	  return 1
      fi
      
      if [ -s stderr ]; then
	  echo "**** Conceal test $TESTNO FAILED - program shouldn't print anything to stderr for this test."
	  FAIL=1
	  return 1
	  
      fi
  fi
  
  if [ -s stdout.txt ]; then
      echo "**** Conceal test $TESTNO FAILED - program shouldn't print any output to stdout."
      FAIL=1
      return 1
  fi
      
  echo "Conceal test $TESTNO PASS"
  return 0
}

# Test the extract program.
testExtract() {
  TESTNO=$1
  BITCOUNT=$2

  rm -f output.txt stdout.txt stderr.txt

  # Recover a message hidden in an image.
  # We have one tests without the right number of command-line argumets.
  if [ $TESTNO == 12 ]; then
      echo "Extract test $TESTNO: ./extract concealed-$TESTNO.ppm output.txt $BITCOUNT extra"
      ./extract concealed-$TESTNO.ppm output.txt $BITCOUNT extra > stdout.txt 2> stderr.txt
      STATUS=$?
  else
      # Recover a message hidden in an image.
      echo "Extract test $TESTNO: ./extract concealed-$TESTNO.ppm output.txt $BITCOUNT"
      ./extract concealed-$TESTNO.ppm output.txt $BITCOUNT > stdout.txt 2> stderr.txt
      STATUS=$?
  fi

  # Make sure the output file looks right.
  if [ -f message-$TESTNO.txt ]; then
      if ! diff -q message-$TESTNO.txt output.txt >/dev/null 2>&1
      then
	  echo "**** Extract test $TESTNO FAILED - output message didn't match the expected output"
	  FAIL=1
	  return 1
      fi
  fi

  # For the program shouldn't print any output to standard output
  if [ -s stdout.txt ] ;then
      echo "**** Extract test $TESTNO FAILED - program shouldn't print any output to stdout."
      FAIL=1
      return 1
  fi
      
  # If an error output file exists.
  if [ -f estderr-$TESTNO.txt ] ; then
      # Make sure we get the right error message and exit unsuccessfully.
      if [ $STATUS != 1 ]; then
	  echo "**** Test $TESTNO FAILED - program should exit unsuccessfully for this test."
	  FAIL=1
	  return 1
      fi
      
      if ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      then
	  echo "**** Extract test $TESTNO FAILED - program didn't print the right error message."
	  FAIL=1
	  return 1
      fi
  else
      # On a successful test, there should be no error output and the program
      # should exit successfully
      if [ $STATUS != 0 ]; then
	  echo "**** Extract test $TESTNO FAILED - program should exit successfully for this test."
	  FAIL=1
	  return 1
      fi
      
      if [ -s stderr ]; then
	  echo "**** Extract test $TESTNO FAILED - program shouldn't print anything to stderr for this test."
	  FAIL=1
	  return 1
	  
      fi
  fi
  
  if [ -s stdout.txt ]; then
      echo "**** Extract test $TESTNO FAILED - program shouldn't print any output to stdout."
      FAIL=1
      return 1
  fi
      
  echo "Extract test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

if [ -x conceal ] ; then
    testConceal 01 01 1
    testConceal 02 01 2
    testConceal 03 01 3
    testConceal 04 01 8
    testConceal 05 01 4
    testConceal 06 02 3
    testConceal 07 03 2

    testConceal 08 90 3
    testConceal 09 91 2
    testConceal 10 92 5
else
    echo "**** Your conceal didn't compile successfully, so we couldn't test it."
    FAIL=1
fi

if [ -x extract ] ; then
    testExtract 01 1
    testExtract 02 2
    testExtract 03 3
    testExtract 04 8
    testExtract 05 4
    testExtract 06 3
    testExtract 07 2

    testExtract 11 9
    testExtract 12 2
else
    echo "**** Your extract didn't compile successfully, so we couldn't test it."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi
