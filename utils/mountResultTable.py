import os
import csv
import numpy as np


def get_min_cost_execution(csv_file):
    data = np.genfromtxt('executions_ils/ils_' + csv_file + '.csv',
                         delimiter=';', dtype=None, names=True, encoding=None)
    cost_index = 1  # Index of the 'custo' column
    time_index = 2  # Index of the 'tempo' column

    # mean_cost = np.mean(data[data.dtype.names[cost_index]])
    # mean_execution_time = np.mean(data[data.dtype.names[time_index]])

    # return np.array([data.dtype.names[0][0], mean_cost, mean_execution_time])

    min_cost = np.min(data[data.dtype.names[cost_index]])
    rows_with_min_cost = data[data[data.dtype.names[cost_index]] == min_cost]

    min_execution_time = np.min(
        rows_with_min_cost[data.dtype.names[time_index]])

    row_with_min_values = rows_with_min_cost[rows_with_min_cost[data.dtype.names[time_index]]
                                             == min_execution_time][0]

    return row_with_min_values


def read_csv_files_and_get_min_values(csv_files):
    min_values = []
    for file in csv_files:
        print(file)
        min_row = get_min_cost_execution(file)
        min_cost = min_row[1]  # Index of the 'custo' column
        min_execution_time = min_row[2]  # Index of the 'tempo' column
        min_values.append({'instancia': os.path.basename(
            file), 'custo': min_cost, 'tempo': min_execution_time})

    return min_values


def save_to_csv(output_file, data):
    with open(output_file, mode='w', newline='') as file:
        fieldnames = ['instancia', 'custo', 'tempo']
        writer = csv.DictWriter(file, fieldnames=fieldnames)

        writer.writeheader()
        writer.writerows(data)


if __name__ == '__main__':
    csv_files = ["n9k5_A", "n9k5_B", "n9k5_C", "n9k5_D", "n14k5_A", "n14k5_B", "n14k5_C", "n14k5_D", "n22k3_A", "n22k3_B", "n22k3_C", "n22k3_D", "n31k5_A", "n31k5_B", "n31k5_C", "n31k5_D",
                 "n43k6_A", "n43k6_B", "n43k6_C", "n43k6_D", "n64k9_A", "n64k9_B", "n64k9_C", "n64k9_D", "n120k7_A", "n120k7_B", "n120k7_C", "n120k7_D", "n199k17_A", "n199k17_B", "n199k17_C", "n199k17_D"]
    # Get min cost and execution time from each file
    min_values_data = read_csv_files_and_get_min_values(csv_files)

    # Save the data to a new CSV file
    output_csv_file = 'tsp_average.csv'
    save_to_csv(output_csv_file, min_values_data)

    print(
        f"Minimum values from each file have been saved to '{output_csv_file}'.")
