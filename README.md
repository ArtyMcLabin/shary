# shary
directly share files between your computers.


----------


Installation: <br>
------------------

>**1)** download "/src" folder contents from this git repository <br>
>**2)** write "make" in terminal ("sudo make" or similar)  <br>
>* **2.1)** optionally if you don't want the executable to be deployed into /usr/bin (enabled by default), run "make link" instead  <br>
>
>**3)** have fun.  <br>

example:

    sudo apt-get install git libboost-all-dev
    cd ~/Downloads
    git clone https://github.com/ArtyMcLabin/shary.git
    cd shary/src
    sudo make
    shary

Usage: <br>
------------------

>###edit contacts: <br>
>*      <b>(desktop):</b>      shary add laptop 192.168.0.2 <br>
>*      <b>(desktop):</b>      shary remove laptop <br>
> 
>###transfer files: <br>
>*      <b>(laptop):</b>       shary get <br>
>*      <b>(desktop):</b>      shary give laptop foo.txt *.jpg <br>
>
>###helper commands: </br>
>*      <b>(desktop):</b>      shary list <br>
>       ><i>outputs list of all contacts and their IP's </i><br>

Notes: <br>
------------------
* This software is redistributed under BSD 3-Clause License.
* Compilation requires GCC (with c++1z support) and Boost library.
* Shary is by now dependent on netcat (nc) for networking. (should be present in most Linux distros by default, else easilly accessed)
