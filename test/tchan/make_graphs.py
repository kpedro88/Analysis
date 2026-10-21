import sys

inname = sys.argv[1]

import array
import os
import ROOT

ytitle = ""

def csv_to_tgraph(csv_filename, output_filename="graphs.root"):
    # Open the ROOT file for writing
    output_file = ROOT.TFile(output_filename, "RECREATE")

    current_name = None
    x_vals = []
    y_vals = []

    def save_current_graph():
        """Helper function to build and write the TGraph to the ROOT file."""
        if current_name and len(x_vals) > 0:
            # PyROOT requires arrays or vectors for pointer-based TGraph constructor
            x_arr = array.array("d", x_vals)
            y_arr = array.array("d", y_vals)

            graph = ROOT.TGraph(len(x_vals), x_arr, y_arr)

            # Sanitize the name so it can be safely stored/retrieved in ROOT
            sanitized_name = (
                current_name.replace(" ", "_").replace("(", "").replace(")", "")
            )
            graph.SetName(sanitized_name)
            graph.SetTitle(current_name)

            # Set axes titles if you want them saved
            graph.GetXaxis().SetTitle("m_{#Phi} [GeV]")
            graph.GetYaxis().SetTitle(ytitle)

            graph.Write()
            print(
                "Saved graph: '{sanitized_name}' with {len_x_vals} points.".format(sanitized_name=sanitized_name, len_x_vals=len(x_vals))
            )

    # Parse the CSV file line by line
    with open(csv_filename, "r") as f:
        for line in f:
            line = line.strip()

            # Empty lines separate blocks
            if not line:
                continue

            if line.startswith("#"): continue

            # Detect a new header block
            if line.startswith("$m_{\\Phi}$"):
                # Save the previous graph before moving to the next one
                save_current_graph()

                # check y title
                if "inv" in line: ytitle = "r_{inv}"
                elif "lambda" in line: ytitle = "#lambda"

                # Clean up the name string as requested
                # Drops "$m_{\Phi}$ [GeV]," and ": $r_{\mathrm{inv}}$"
                name = (
                    line.replace("$m_{\\Phi}$ [GeV],", "")
                    .replace(": $r_{\\mathrm{inv}}$", "")
                    .replace(": $\lambda$", "")
                    .strip()
                )

                current_name = name
                x_vals = []
                y_vals = []

            # Process data points
            elif "," in line and current_name:
                x_str, y_str = line.split(",")

                # Skip entries with '-'
                if y_str.strip() == "-":
                    continue

                try:
                    x_vals.append(float(x_str.strip()))
                    y_vals.append(float(y_str.strip()))
                except ValueError:
                    # Skips unexpected lines or broken floats safely
                    continue

        # Save the final block in the file
        save_current_graph()

    output_file.Close()
    print("\nSuccessfully wrote all graphs to {output_filename}".format(output_filename=output_filename))


if __name__ == "__main__":
    csv_to_tgraph(inname, inname.replace(".csv",".root"))
