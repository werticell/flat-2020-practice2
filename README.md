# Second formal languages practicum task
Реализовать алгоритм Эрли, который принимает грамматику и произвольное слово, и отвечает 
принадлжит ли данное слово языку задаваемому полученной грамматикой или нет.

## Solution 
Алгоритм Эрли без оптимизаций за куб. ([Статья про алгоритм Эрли](http://www.mi-ras.ru/~sk/lehre/fivt2013/Earley.pdf))



### Building and testing
You will need [googletest](https://medium.com/@alexanderbussan/getting-started-with-google-test-on-os-x-a07eee7ae6dc) 
to build and test it.

To build project.
````
    git clone <github link>
    cd mipt-flat-2020-practice2
    ./build.sh
````

To run tests after building. (You can write your own tests in main_test.cpp.)
````
    cd bin
    ./Test
````
To run program after building.
````
    cd bin
    ./mipt_flat_practice2
````

 