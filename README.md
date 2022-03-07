# SaAlign

## Libraries that Need to be Pre Installed
[pyspark](https://spark.apache.org/docs/latest/api/python/) ---must be compatible with the corresponding Python version to run

PySpark is an interface for Apache Spark in Python. It not only allows you to write Spark applications using Python APIs, but also provides the PySpark shell for interactively analyzing your data in a distributed environment. PySpark supports most of Spark’s features such as Spark SQL, DataFrame, Streaming, MLlib (Machine Learning) and Spark Core.

**An example (Word Count) using PySpark**

```py
import sys
from operator import add

from pyspark.sql import SparkSession


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: wordcount <file>", file=sys.stderr)
        sys.exit(-1)

    spark = SparkSession\
        .builder\
        .appName("PythonWordCount")\
        .getOrCreate()

    lines = spark.read.text(sys.argv[1]).rdd.map(lambda r: r[0])
    counts = lines.flatMap(lambda x: x.split(' ')) \
                  .map(lambda x: (x, 1)) \
                  .reduceByKey(add)
    output = counts.collect()
    for (word, count) in output:
        print("%s: %i" % (word, count))

    spark.stop()

```

## Instruction
1. Downlosd ***SaAlign_package-1.0.tar.gz*** from website： https://github.com/YangyiLab/SaAlign.
2. Unzip the downloaded file to a folder.
3. Enter the folder directory, open the terminal and enter the command :`sudo python setup.py`.
4. When writing Python scripts,Using Python statements: `from SaAlign.SaAlign import sa_align`, Using sa_ align for MSA operation.

## Parameter details:

### Local Node Examples

```py
sa_align(memory,application_name,filename,basepath,partition = 5,url = "local[*]")
```

### Spark Cluster Example

```py
sa_align(memory,application_name,filename,basepath,partition = 5,url = "url of your spark cluster")
```

## Parameter description

### memory
The memory allocated by each executor. The allocation of this parameter will affect the running success or not.
eg."2g","512m"
### application_name
The name of the starting task.
eg."First_App"
### filename
Name of fasta file for MSA operation
eg."virus.fa"
### basepath
After extracting ***SaAlign_package-1.0.tar.gz*** ,the path of ***SaAlign.py*** 
### partition
The number of all partitions specified when running the program,which can be specified by yourself
default = 5 
### url
When running the program, the URL address of the spark cluster (eg."spark://192.132.232.1:7077"). The default address is localhost,that is, ```local [*]``` using all CPU cores for calculation.


## Output

+ Sequence file after completing MSA (Multiple Sequence Analysis)
+ Plus, you can use the output file of SaAlign to construct the Polyogenetic Tree with any tool you want like (Raxml, IQTree and etc.)

## Known issues and planned updates
1. Known issues

+ There must be a line gap between each sequence in the fasta files.

+ The accuracy is good especially for the sequences which are quite different with each other but in this consequence the speed is relatively slow.
 
1. TODO list
 
+ Offer more options to select the algorithm

## Contact
For bug reports or suggestions, please contact me at princeyuansql AT gmail DOT com
