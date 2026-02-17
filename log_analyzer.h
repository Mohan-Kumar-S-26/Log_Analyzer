#ifndef LOG_ANALYZER_H
#define LOG_ANALYZER_H

#define MAX_LINE 200
#define MAX_LEVEL 20

typedef struct {
    char level[MAX_LEVEL];
    int count;
} Severity;

// Function Declarations
int loadConfig(const char *filename, Severity **levels);
int analyzeLogs(const char *logFile, Severity *levels, int n);
void sortByFrequency(Severity *levels, int n);
void generateReport(const char *outputFile, Severity *levels, int n, int total);

#endif