#!/bin/bash
ws=/home/txt/result/

for i in bedroom5 lab605_3 strorageroom stair4 photobooth5 cafe10 fakeface5 fakeface3 bedroom_countryside livingroom_countryside fifthfloor lab605 campus1 bookstore9 bedroom3 stair3 livingroom3; do

    DATASET_PATH=$ws$i
    # mkdir $DATASET_PATH/semantic
    # mkdir $DATASET_PATH/semantic/org_binary_mask
    # mkdir $DATASET_PATH/semantic/images_masked

    echo $DATASET_PATH/
    for depthHD in $DATASET_PATH/depthHD*
    do
        num_chosen=${depthHD%*.png}
        num_chosen=${num_chosen#*depthHD_}
        echo $num_chosen
        mask=$DATASET_PATH/depthHD_${num_chosen}.png
        rgb_save_path=$DATASET_PATH/RGB_fix_hole_${num_chosen}.png
        rgb_image_with_hole=$DATASET_PATH/RGB_${num_chosen}.png
        rgb_image_reference=$DATASET_PATH/RGB_render_${num_chosen}.png

        sed -i "8c rgb_save_path: $rgb_save_path"   /home/txt/HW3/combine2images/fix_hole_with_render.yaml
        sed -i "7c rgb_image_with_hole: $rgb_image_with_hole"   /home/txt/HW3/combine2images/fix_hole_with_render.yaml
        sed -i "6c rgb_image_reference: $rgb_image_reference"  /home/txt/HW3/combine2images/fix_hole_with_render.yaml
        sed -i "5c mask_path: $mask"   /home/txt/HW3/combine2images/fix_hole_with_render.yaml    
        /home/txt/HW3/combine2images/build/general_fuctions /home/txt/HW3/combine2images/fix_hole_with_render.yaml


    done
done