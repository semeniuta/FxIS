import numpy as np
import pandas as pd
from matplotlib import pyplot  as plt

import fxisext

def create_writedf_from_image_response(resp):

    t_snap = np.array(fxisext.get_timestamps_snaphot(resp))
    return pd.DataFrame(t_snap, columns=['t_frame_arrived', 't_frame_processed'])


def transform_writedf_to_timestamp_seq(df_full, index_from=None, index_to=None):
    '''
    Transfrom the whole write_df (or its part) to a sequence of
    timestamps such as:
    ( ... t_frame_arrived[i],     't_frame_processed[i],
          t_frame_arrived[i + 1], 't_frame_processed[i + 1], ... )
    '''

    sz = df_full.shape[0]

    index_from, index_to = set_boundary_indices(index_from, index_to, sz)

    df = df_full[index_from:index_to+1]

    data_type = df_full['t_frame_arrived'].dtype
    res = np.zeros(df.shape[0] * 2, dtype=data_type)

    ind = 0
    for i in range(sz):
        for colname in ('t_frame_arrived', 't_frame_processed'):
            res[ind] = df[colname][i]
            ind += 1

    return res


def create_relative_timestamp_seq(seq, zero_val=None):
    '''
    Given any timestamp sequnce, create the corresponding
    sequence of durations computed with respect to the
    0'th element of the sequence or other provided zero value
    '''

    res = np.zeros(len(seq), dtype=seq.dtype)

    if zero_val is None:
        zero_val = seq[0]

    for i, val in enumerate(seq):
        res[i] = val - zero_val

    return res


def set_boundary_indices(index_from, index_to, n):

    a, b = index_from, index_to

    if index_from is None:
        a = 0

    if index_to is None:
        b = n - 1

    return a, b


def visualize_writes(ax, seq, index_from=None, index_to=None):

    assert len(seq) % 2 == 0

    num_writes = len(seq) // 2

    index_from, index_to = set_boundary_indices(index_from, index_to, num_writes)

    for i in range(index_to, index_from - 1, -1):

        index_in_seq = i * 2

        ax.bar([0], seq[index_in_seq + 1], color='deeppink')
        ax.bar([0], seq[index_in_seq], color='lightcyan')

    ax.set_xticks([])


def save_timestamps_snapshot_viz(responses, file_name, figure_size=(5, 15)):

    n_cams = len(responses)

    fig, axes = plt.subplots(1, n_cams, sharey=True, figsize=figure_size, gridspec_kw={'wspace':0, 'hspace':0})

    for i, resp in enumerate(responses):

        ax = axes[i]

        df_write = create_writedf_from_image_response(resp)
        write_seq = create_relative_timestamp_seq(transform_writedf_to_timestamp_seq(df_write))
        visualize_writes(ax, write_seq * 1e-6)

    plt.tight_layout()
    plt.savefig(file_name)
