## Initialization ##
cd input_directory
set datafile separator ","

## Graph Formatting ##
set title 'Runtime'
set xlabel 'Batch Size'
set ylabel 'Input Size'
set zlabel 'Time (in seconds)' offset 0,-8

set hidden3d
set grid x y z
set size ratio -1
set logscale xyz 2
set format x "2^{%L}"
set format y "2^{%L}"
set format z "2^{%L}"
set xtics offset 0, -.4
set ytics offset 0, -.4
set key autotitle columnhead
set term postscript enhanced color solid

## Plotting ##
do for [i=0:5] {
	t = 2**i
	list = system(sprintf('ls *-%d-thread.csv 2>/dev/null', t))
	if (words(list) == 3) {
		set output sprintf("naive-verifier-%d.ps", t)
		splot sprintf('naive-%d-thread.csv', t) using 1:2:5 with lines title sprintf('naive (%d-thread)', t),\
		sprintf('verifier-%d-thread.csv', t) using 1:2:5 with lines title sprintf('verifier (%d-thread)', t)

		set output sprintf("naive-prover-%d.ps", t)
		splot sprintf('naive-%d-thread.csv', t) using 1:2:5 with lines title sprintf('naive (%d-thread)', t),\
		sprintf('prover-%d-thread.csv', t) using 1:2:5 with lines title sprintf('prover (%d-thread)', t)
	}
}

list = system('ls naive-*.csv 2>/dev/null')
if (words(list) > 1) {
	set output "naive.ps"
	splot for [i=1:words(list)] word(list,i) using 1:2:5 with lines title word(list,i)
}

list = system('ls prover-*.csv 2>/dev/null')
if (words(list) > 1) {
	set output "prover.ps"
	splot for [i=1:words(list)] word(list,i) using 1:2:5 with lines title word(list,i)
}

list = system('ls verifier-*.csv 2>/dev/null')
if (words(list) > 1) {
	set output "verifier.ps"
	splot for [i=1:words(list)] word(list,i) using 1:2:5 with lines title word(list,i)
}
