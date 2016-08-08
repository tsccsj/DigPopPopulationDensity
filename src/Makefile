
GCC	?= g++

# Gdal
GDALDIR = /sw/geosoft/gdal-1.11.2-fgdb
GDALLIB := -L$(GDALDIR)/lib -lgdal

# Target rules
all: DigPopPopulationDensity

io.o: io.c
	$(GCC) -I. -I$(GDALDIR)/include -o $@ -c $< 
kde.o: kde.c
	$(GCC) -I. -o $@ -c $<
digPopPopulationDensity.o: digPopPopulationDensity.c
	$(GCC) -I. -o $@ -c $<
DigPopPopulationDensity: io.o kde.o digPopPopulationDensity.o
	$(GCC) -o ../$@ $+ $(GDALLIB) 
clean:
	rm -f ../DigPopPopulationDensity *.o
