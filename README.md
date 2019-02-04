# MiniOS

An instructional operating system for ARM CPUs (The Atmel SAM4S MCU, and Raspbery PI 2 and 3)

<p align="center">
  <img src="https://github.com/rromanotero/minios/blob/master/logo.png" width="290"/>
  <p align="center">It's supposed to be a "wireless" mug!</p>
</p>


# Origins

MiniOS was originally written as an instructional OS for teaching Operating Systems Labs at the University of Northern BC. The first version started on 2014, and it's being refined on every offering ever since. (Currently I'm trying to adapt the material for Raspberry PI)


# Use

The OS is intended to be delivered together with *the MiniOS Book*. A book showing step by step how to build MiniOS from scratch. So you need to have the book to make sense of this code.

How to get the book? Well, the book is for the most part complete, but my co-author (Dr. Alex Aravind) and me have been busy and are still working out the final details (as of 2019). Should you want a copy for academic purposes or just for yourself to build your own OS, feel free to send me an e-mail! 

Bare in mind MiniOS and the book have both been edited since the paper publications (section have been added and sections have been dropped out). So, there's some mitmatch between all the sections as described in the MiniOS papers, and the ones you'll see in the book (the main ones are still there, though). 


# Can a microcontroller be used to teach Operating Systems?

Yes (arguably). More details in the arXiv paper, see below. We also have a paper where we argue that it's possible to teach Computer Architecture (for a computer science course) with a MCU platform. More details in the publication section, below. 


# MiniOS Apps

One important aspect of MiniOS is that it ca run actual "apps". Separately-written and separately-deployable apps, like in a general purpose computer system. These are two examples of apps runnnig from the console [Example 1](https://youtu.be/xwV5s2CTYqg) and [Example 2](https://youtu.be/HzgY5F7Pba4)


# What is it used for (other than instruction)?

One of the design goals of MiniOS was that it should be "functional". Meaning, one should be able to use it to build stuff. So, even though, MiniOS is very minimal, and you could say "it's missing parts", you can still use it. It works! The parts that are written do not randomly break. As long as you are aware of certain constraints (e.g. don't go beyond the max number of processes; or tell the kernel to run something that isn't an app; that'll break it!).


What exactly can you build? Still exploring this. One example is MCU robots, such as [this MiniOS-based robot](https://www.youtube.com/watch?v=Tp8fJ_0Ap_s). 


# On what platforms does it run?

Currently the SAM4S Xplained Pro, but I'm currently attempting x86.


# Compiling Source code

Source code is in the form of an Atmel Studio Project. More details on how to compile it and use it are covered in the book.


# "So, if I were to use it, what would I need?"

You'd need:

 - The MiniOS Book
 - A lab instructor (or TA) that either has some experience or at least understands how to operate a computer at a very low-level; who is also very patient. Concurrent low-level software is hard!
 - A copy of this source code
 - An Atmel SAM4S Xplained Pro boards ([See here](https://www.microchip.com/DevelopmentTools/ProductDetails/atsam4s-xpro)) per team of students or per student. With a few Extension Kits, if possible (not required). I suggest the IO1, OLED1, and the BNO055 (you can build some cool robots with the BNO055). 
 

# Publications and arXiv

Rafael Roman Otero and Alex Aravind. 2018. MiniOS: an instructional platform for teaching operating systems labs. arXiv:1811.09792, Dec 2018. [PDF here](https://arxiv.org/abs/1811.09792v2)

Rafael Roman Otero and Alex Aravind. 2015. MiniOS: An Instructional Platform for Teaching Operating Systems Projects. In Proceedings of the 46th ACM Technical Symposium on Computer Science Education (SIGCSE ’15). ACM, New York, NY, USA, 430-435. [PDF here](http://embedntks.com/datastructs/papers/minios%20paper%20%28published%20version%29.pdf)

This is the one about teaching computer architecture:

Rafael Roman Otero and Alex Aravind. 2018. Teaching Computer Architecture Labs using a MCU Platform. In Proceedings of the 23rd Western Canadian Conference on Computing Education (WCCCE '18). ACM, New York, NY, USA, Article 3, 6 pages. [PDF here](http://embedntks.com/datastructs/papers/wccce%20teaching%20computer%20architecture%20%28published%20version%29.pdf)

