## Test environments

* local OS X install, R 3.6.1 and 4.0.0
* win-builder (devel and release)

## R CMD check results

There were no ERRORs or WARNINGs. 

## devtools check results

This warning only shown in R 4.0.0

checking top-level files ... WARNING
  A complete check needs the 'checkbashisms' script.
  See section ‘Configure and cleanup’ in the ‘Writing R Extensions’
  manual.

## rhub check results

- Debian Linux, R-devel, GCC ASAN/UBSAN OK

- Ubuntu Linux 16.04 LTS, R-release, GCC 1 NOTE
GNU make is a SystemRequirements.

- Fedora Linux, R-devel, clang, gfortran
* checking top-level files ... WARNING
A complete check needs the 'checkbashisms' script.
See section ‘Configure and cleanup’ in the ‘Writing R Extensions’
manual.

- Windows Server 2008 R2 SP1, R-devel, 32/64 bit

I think it says ok...

## Previous changed

1. Include method description link in the Desciption
2. Fix a cran link problem in README.md
3. Add single quotes in title and description in the DESCRIPTION file
4. Change T to TRUE and F to FALSE
5. Did not find any variables in my side named T or F
6. Add \value Rd-tags in      
     - MrSFit.Rd: \value
     - MrSImp.Rd: \value
     - plotTree.Rd: \value
     - predictTree.Rd: \value
     - printTree.Rd: \value
     - writeTex.Rd: \value
7. remove some functions export, since they should not be used directly by users.
8. remove \dontrun{} block
9. In print.R/writeTex.R, added \dontshow{.old_wd <- setwd(tempdir())} and \dontshow{setwd(.old_wd)} around the writting function.
10. Changed plot.R `cat` to `warining`.
11. fix a bug in regression.cpp about log(int n), cast to log (double n). It fixed the issue in Solaris.
12. fix a bug in mrsguide.R and mrsimp.R, when Y is single outcome.
12. Fix URLs in README.md and DESCRIPTION



