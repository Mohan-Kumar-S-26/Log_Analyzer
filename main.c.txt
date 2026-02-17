#include <stdio.h>
#include <stdlib.h>
#include "log_analyzer.h"

int main() {
    Severity *levels = NULL;
    printf("Starting Log Analyzer...\n");

  
    int n = loadConfig("config.txt", &levels);
    if (n == 0) return 1;

    
    int totalLogs = analyzeLogs("logs.txt", levels, n);

   
    sortByFrequency(levels, n);

   
    generateReport("report.txt", levels, n, totalLogs);

    printf("Analysis Complete! Checked %d logs.\n", totalLogs);
    printf("Open 'report.txt' to see the results.\n");

    free(levels);
    return 0;
}