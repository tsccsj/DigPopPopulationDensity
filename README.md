# DigPopPopulationDensity

PopulationDensity is software developed to generate population density maps of digital population realizations using kernel density estimation.

# Software instruction
http://digitalpopulations.pbworks.com/w/page/110076190/PopulationDensity

# What does it do?
For each Digital Populations realizations, it generates either:
  A density map of households in the realization; or
  A density map of individual persons in the realization
as specified by the user

# Prerequisites
  GDAL

# To run
Find the compiled executable (i.e. DigPopPopulationDensity) and run it with the following arguments:
DigPopPopulationDensity OutputFileName InputDigPopNamePreNumber MinRzn NumberRzn ProjectionCode NorthBounds EastBounds SoutBound WestBounds LatticeSize KernelDiameter HouseholdOrPopulationMap PersonsPerHousehold(For population density)

Input arguments:
 1. OutputFileName: the name and directory of output files. It is assumed that output maps will be named as 'OutputFile'_rzn'RznNumber'.tif
 2. InputDigPopNamePreNumber: the name and directory of input Digital Populations realizations files.
  i. It is assumed that input household files will be named 'InputFile''RznNumber'-households.csv 
 3. MinRzn: the index of the first realization to be analyzed
 4. NumRzn: the number of realizations to be analyzed
  i. Realization number is assumed to have three digit in file names 
 5. ProjectionCode: the EPSG code indicating the projection information of input coordiates
 6. NorthBounds: the north bound (Max of Y) of study area
 7. EastBounds: the east bound (Max of X) of study area 
 8. SouthBounds: the north bound (Min of Y) of study area
 9. WestBounds: the north bound (Min of X) of study area
 10. LatticeSize: the cell size of output raster maps
 11. KernelBandwidth: the bandwidth used by the kernel density estimation
  i. If a zero(0) bandwidth is used, the code simply count the number of persons (households) in each cell and calculate the ratio
 12. HouseholdOrPopulationMap: the indicator of whether the analysis is based on household or population
  i. H for household. Thus it will calculate the density map of households
  ii. P for population. Thus it will calculate the density map of individual persons
 13. PersonsPerHousehold:  the column name in the household file which represents the the number of people in each household. This parameter is needed only for population density mapping. 
