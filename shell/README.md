# shell

This was my final project for my Operating Systems class at NMU. We were asked to build a shell and add features to it from a list we were given, up to 20 points. Here are the features I added, which is every command that my shell can execute and run.<br/>
<br/>
  1 Can run an executable.  I type in some full path, and you exec it.<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/bin/ls<br/>
  1 You search the path for the executable<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ls<br/>
  1 Can do file input redirection "<"<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ls > fred<br/>
  1 Can do file output redirection ">"<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ls < fred<br/>
  2 Can set enviornment variables<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(See https://www.greenend.org.uk/rjk/tech/putenv.html)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PATH=:/bin:/sbin:/usr/sbin:/usr/bin<br/>
  2 Expands enviornment variables on the command line<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Either manually or with wordexp)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ls $HOME<br/>
  2 Does filename expansion "glob" <br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Hint:  Use the built in glob, or wordexp.)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(http://www.gnu.org/software/libc/manual/html_node/Wordexp-Example.html)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ls a*b<br/>
  1 Knows how to change directory<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(See chdir)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;cd /fred<br/>
  1 Queue commands<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;make ; make install<br/>
  +1 Can have lots of semicolons<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ls; sleep 3; rm fred<br/>
  2 Change Prompt<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PS1="what is you command?"<br/>
  1 Catch Keyboard interrupt<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(See https://www.geeksforgeeks.org/signals-c-language/)<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ctrl + c = back to prompt<br/>
  1 Control-L clears the screen<br/>
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ctrl-l = clear screen<br/>
  2 Tab Completion and Arrow History (see the readline library)<br/>
  1 Saves and reloads history to a file<br/>
  <br/>
  <br/>
In this project I used the readline library, it was the first time for me, I found it really interesting and it made everything so much easier. <br/>
<br/>
I also found really useful using the build in function wordexp, it made some of the points easier than it would have been doing them manually.<br/>
<br/>
In addition, all the executable is run by using the execvp function, it took me a little to understand how it worked, but once I did I managed to get the biggest part of the project working.<br/>
<br/>
To end up, in this project I used a part of the C language that I didn't know about and it can be really useful, signals. I used it to catch the keyboard interrupt build in the readline library (ctr+C) which terminates the program. In this case, it ends when the user types "quit".<br/>
