<?php 
  get_header(); 
?>

<h1>Add your message here</h1>

<form class="message-form" method="post">
  <input id="message-title" type="text">
  <button  type="submit">Save!</button>
</form>




<div>
    <?php
      require 'inc/section_vars.php';   
      // get_example_data is in /inc/template_functions.php
      $data  = get_example_data($example_repeater);
      if(!empty( $data ) ) { 
        ?>
        <section class="example">
        <?php
          foreach ( $data as $k => $f ) {
            $media = '';
            if ($f['some_image']) {
              // get_media_url function is in template_functions.php
              $media = '<img src="'.esc_url( get_media_url( $f['some_image'] ) ).'">';
            }
        ?>
            <div><?php echo $media ?></div>
            <div><?php echo $f['some_quote'] ?></div>

    <?php
          }
    ?>
        </section>
<?php } ?>

</div>

<?php get_footer(); ?>
