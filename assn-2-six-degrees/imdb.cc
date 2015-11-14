using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "imdb.h"
#include <string.h>

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

struct Key {
  const char* keyString;
  const void* data;

};

int compareFn(const void* key, const void* elem) {
  Key* myKey = (Key*)key;
  int offset = *(int*)elem;
  return strcmp(myKey->keyString, (char*)myKey->data + offset);
}


imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const {
  // return false;
  int len = *(int*)actorFile;
  Key key;
  key.keyString = player.c_str();
  key.data = actorFile;
  int* ptr = (int*)bsearch(&key, (char*)actorFile + sizeof(int), len, sizeof(int), compareFn);
  int location = *ptr;
  if (ptr == NULL) {
    return false;
  }
  
  int keyLength = strlen(key.keyString);
  int buffer = keyLength + 1;
  buffer += buffer % 2;
  //int pos = buffer + location;
  short numFilms;
  memcpy(&numFilms, (char*)actorFile + buffer + location, sizeof(short));
  buffer += sizeof(short);

  buffer += buffer % 4;
  for (int i = 0; i < numFilms; ++i) {
    int movieOffset;
    memcpy(&movieOffset, (char*)actorFile + buffer + location  + i*sizeof(int), sizeof(int));
    char *name = (char*)movieFile + movieOffset;
    int year = 1900 + *((char*)movieFile + movieOffset + strlen(name) + 1);
    // int numOffset = movieOffset + strlen(name) + 1 + sizeof(char);
    //numOffset += numOffset % 2;
    //short numActors;
    //memcpy(&numActors, (char*)movieFile + numOffset, sizeof(short));
    //int dataOffset = numOffset + sizeof(short);
    //dataOffset += dataOffset % 4;
    film newFilm;
    newFilm.title = std::string(name);
    newFilm.year = year;
    //newFilm.numActors = numActors;
    //newFile.actorOffset = dataOffset;
    films.push_back(newFilm);
  }
  return true;
}
/*bool imdb::getCredits(const string& player, vector<film>& films) const {

    // Copy into numRecords, sizeof(int) bytes, starting at memory address actorFile
    int numRecords;
    memcpy(&numRecords, actorFile, sizeof(int) );

    Key needle;
    needle.keyString = player.c_str();
    needle.data = actorFile;

    int* ptr = (int*)bsearch( &needle, (char*)actorFile+sizeof(int), numRecords, sizeof(int), compareFn );
    if( ptr == NULL ) {
        return false;
    }

    int location = *ptr;
    void* actorRecord = (char*)actorFile+location;
    char* actorName = (char*)actorRecord;

    // Figure out where the num of movies info is stored by looking for it after the actors name
    int buffer = strlen( actorName )+1;
    // If the actors name is not an even number of characters, padd 1 extra byte
    buffer += buffer % 2;

//	std::cout << "\t actorName =" << actorName << std::endl;

    short numMovies;
    memcpy(&numMovies, (char*)actorRecord+buffer, sizeof(short) );

    int movieBuffer = (buffer + sizeof(short));
    movieBuffer += movieBuffer % 4;

    int j = 0;
    for(j = 0; j < numMovies; ++j ) {
        int movieRecordOffset;
        memcpy(&movieRecordOffset, (char*)actorRecord+movieBuffer + sizeof(int)*j, sizeof(int) );

        char* movieName = (char*)movieFile+movieRecordOffset;
        int movieYear = 1900 + *((char*)movieFile+movieRecordOffset + strlen(movieName) + 1);

        // Pad 1 more byte for the char, if it's not even add another extra byte
        int actorPadding = movieRecordOffset + ( strlen(movieName) + 1) + sizeof(char);
        actorPadding += actorPadding%2;

        short numActors = 0;
        memcpy(&numActors, (char*)movieFile+actorPadding, sizeof(short) );

        int dataPadding = actorPadding + sizeof(short);
        dataPadding += dataPadding % 4;

        film aFilm;
        aFilm.title = std::string(movieName);
        aFilm.year = movieYear;
        //aFilm.offset = movieRecordOffset;
        //aFilm.actorOffset = dataPadding;
        //aFilm.numActors = numActors;

        films.push_back(aFilm);
    }

    return true;
}*/
bool imdb::getCast(const film& movie, vector<string>& players) const {
  // return false;
  int len = *(int*)movieFile;
  Key key;
  key.keyString = movie.title.c_str();
  key.data = movieFile;
  int* ptr = (int*)bsearch(&key, (char*)movieFile + sizeof(int), len, sizeof(int), compareFn);
  int location = *ptr;
  if (ptr == NULL) {
    return false;
  }
  
  int keyLength = strlen(key.keyString);
  int buffer = keyLength + 1 + sizeof(char);
  buffer += buffer % 2;
  //int pos = buffer + location;
  short numActors;
  memcpy(&numActors, (char*)movieFile + buffer + location, sizeof(short));
  buffer += sizeof(short);
  /* can be replaced by "buffer += buffer % 4;" since buffer is bound to be even
  if (buffer%4 != 0) {
    buffer += 2;
    }*/
  buffer += buffer % 4;
  for (int i = 0; i < numActors; ++i) {
    int actorOffset;
    memcpy(&actorOffset, (char*)movieFile + buffer + location  + i*sizeof(int), sizeof(int));
    char *name = (char*)actorFile + actorOffset;
    //int year = 1900 + *((char*)movieFile + movieOffset + strlen(name) + 1);
    /*int numOffset = movieOffset + strlen(name) + 1 + sizeof(char);
    numOffset += numOffset % 2;
    short numActors;
    memcpy(&numActors, (char*)movieFile + numOffset, sizeof(short));
    int dataOffset = numOffset + sizeof(short);
    dataOffset += dataOffset % 4;*/
    
    //newFilm.title = name;
    //newFilm.year = year;
    //newFilm.numActors = numActors;
    //newFile.actorOffset = dataOffset;
    //films.push_back(newFilm);
    players.push_back(std::string(name));
  }
  return true;
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
