import pandas as pd
import numpy as np
import uproot


def write_vtu(df, filename):
    n = len(df)

    points = "\n".join(
        f"{row.x_mm} {row.y_mm} {row.z_mm}"
        for row in df.itertuples()
    )

    pe_data = "\n".join(str(v) for v in df["energyPE_eV"])
    ke_data = "\n".join(str(v) for v in df["energyKE_eV"])

    vtu = f"""<?xml version="1.0"?>
    <VTKFile type="UnstructuredGrid" version="0.1" byte_order="LittleEndian">
    <UnstructuredGrid>
        <Piece NumberOfPoints="{n}" NumberOfCells="{n}">

        <PointData>
            <DataArray type="Float64" Name="energyPE_eV" format="ascii">
            {pe_data}
            </DataArray>
            <DataArray type="Float64" Name="energyKE_eV" format="ascii">
            {ke_data}
            </DataArray>
        </PointData>

        <Points>
            <DataArray type="Float64" NumberOfComponents="3" format="ascii">
            {points}
            </DataArray>
        </Points>

        <Cells>
            <DataArray type="Int32" Name="connectivity" format="ascii">
            {" ".join(str(i) for i in range(n))}
            </DataArray>
            <DataArray type="Int32" Name="offsets" format="ascii">
            {" ".join(str(i+1) for i in range(n))}
            </DataArray>
            <DataArray type="UInt8" Name="types" format="ascii">
            {"1 " * n}
            </DataArray>
        </Cells>

        </Piece>
    </UnstructuredGrid>
    </VTKFile>"""

    with open(filename, "w") as f:
        f.write(vtu)

    return



def report_energy(df):
    with uproot.open("output.root") as f:
        tree = f["photons"]
        df_photons = tree.arrays(library="pd")
        tree= f["escaped"]
        df_escaped = tree.arrays(library="pd")

    total_energy = df["energyPE_eV"].sum() + df["energyKE_eV"].sum() + df_photons['energy'].sum() + df_escaped['energy'].sum()
    print(f"Total potential energy: {df['energyPE_eV'].sum()} eV")
    print(f"Total kinetic energy: {df['energyKE_eV'].sum()} eV")
    print(f"Total photon energy: {df_photons['energy'].sum()} eV")
    print(f"Total escaped energy: {df_escaped['energy'].sum()} eV")
    print(f"Total energy: {total_energy} eV")
    return


def main():
    df = pd.read_csv("voxel_map.txt", comment="#")
    report_energy(df)
    write_vtu(df, "voxel_map.vtu")



if __name__ == "__main__":
    main()