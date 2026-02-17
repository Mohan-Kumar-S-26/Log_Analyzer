#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log_analyzer.h" // Connects to the header

// Helper function (Internal use only)
void removeNewline(char *str) {
    str[strcspn(str, "\r\n")] = '\0';
}

int loadConfig(const char *filename, Severity **levels) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        return 0;
    }

    char line[MAX_LINE];
    int count = 0;

    // First pass: Count lines
    while (fgets(line, sizeof(line), file)) count++;

    rewind(file);

    // Allocate memory
    *levels = (Severity *)malloc(count * sizeof(Severity));

    // Second pass: Read words
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        removeNewline(line);
        strcpy((*levels)[i].level, line);
        (*levels)[i].count = 0;
        i++;
    }

    fclose(file);
    return count;
}

int analyzeLogs(const char *logFile, Severity *levels, int n) {
    FILE *file = fopen(logFile, "r");
    if (file == NULL) {
        printf("Error: Could not open %s\n", logFile);
        return 0;
    }

    char line[MAX_LINE];
    char firstWord[MAX_LEVEL];
    int total = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%s", firstWord);
        
        for (int i = 0; i < n; i++) {
            if (strcmp(firstWord, levels[i].level) == 0) {
                levels[i].count++;
                total++;
                break;
            }
        }
    }
    fclose(file);
    return total;
}

void sortByFrequency(Severity *levels, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (levels[j].count < levels[j + 1].count) {
                Severity temp = levels[j];
                levels[j] = levels[j + 1];
                levels[j + 1] = temp;
            }
        }
    }
}

void generateReport(const char *outputFile, Severity *levels, int n, int total) {
    FILE *file = fopen(outputFile, "w");
    if (file == NULL) return;

    fprintf(file, "Log Analysis Report\n-------------------\n");
    for (int i = 0; i < n; i++) {
        float percent = (total > 0) ? (levels[i].count * 100.0) / total : 0;
        fprintf(file, "%-10s : %d (%.2f%%)\n", levels[i].level, levels[i].count, percent);
    }
    fprintf(file, "\nTotal Logs: %d\n", total);
    fclose(file);
}