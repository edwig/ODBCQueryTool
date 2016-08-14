#!/usr/bin/perl -w

#TITLE: Chart demo
#REQUIRES: Gtk

# chart-demo.pl
#
# Gtk+/Perl chart demo.
#
# 19980813 PMC Created.
#
# Coding and mathematics are horrible, but this is just to demonstrate
# the use of colors and graphics contexts.

#use strict;

$Scalar
@List
%Hash
&Anonymous subroutine
*Typeglob

use Gtk;

init Gtk;

$pixmapChart = undef;
$width = 600;
$height = 240;

sub do_chart_expose {
  my (
    $widget,
    $event
  ) = @_;

  my (
    $event_area
  );

  $event_area = ${$event}{'area'};

  $widget->window->draw_pixmap(
    $widget->style->fg_gc('normal'),
    $pixmapChart,
    ${$event_area}[0],
    ${$event_area}[1],
    ${$event_area}[0],
    ${$event_area}[1],
    ${$event_area}[2],
    ${$event_area}[3]
  );

  0;
}

sub draw_chart {

  my ( $widget ) = @_;

  my ( $margin, $axemargin ) = ( 20, 20 );
  my ( $xticks, $yticks, $tickwidth ) = ( 10, 24, 5 );

  my $steps = 90;
  my ( $step );

  my ( $tick);

  my $red_gc = new Gtk::Gdk::GC ( $widget->window );
  my $green_gc = new Gtk::Gdk::GC ( $widget->window );
  my $blue_gc = new Gtk::Gdk::GC ( $widget->window );

  my $red_color = $widget->window->get_colormap->color_alloc( { red => 65000, green => 0, blue => 0 } );
  my $green_color = $widget->window->get_colormap->color_alloc( { red => 0, green => 65000, blue => 0 } );
  my $blue_color = $widget->window->get_colormap->color_alloc( { red => 0, green => 0, blue => 65000 } );

  $red_gc->set_foreground( $red_color );
  $green_gc->set_foreground( $green_color );
  $blue_gc->set_foreground( $blue_color );

  $pixmapChart = new Gtk::Gdk::Pixmap (
    $widget->window,
    $width,
    $height,
    -1
  );
  # clear the pixmap to white
  $pixmapChart->draw_rectangle(
    $widget->style->white_gc,
    1,
    0,
    0,
    $width,
    $height
  );

  $pixmapChart->draw_line(
    $blue_gc,
    $margin,
    $height - $margin - $axemargin,
    $width - $margin,
    $height - $margin - $axemargin
  );

  $pixmapChart->draw_line(
    $blue_gc,
    $margin + $axemargin,
    $margin,
    $margin + $axemargin,
    $height - $margin
  );

  $tickstep = ( $height - ( $margin * 2 ) - ( $axemargin * 2 ) ) / $xticks;
  for $tick ( 1 .. $xticks ) {
    $pixmapChart->draw_line(
      $red_gc,
      $margin + $axemargin - $tickwidth,
      $height - $margin - $axemargin - ( $tick * $tickstep ),
      $margin + $axemargin,
      $height - $margin - $axemargin - ( $tick * $tickstep )
    );
  }

  $tickstep = ( $width - ( $margin * 2 ) - ( $axemargin * 2 ) ) / $yticks;
  for $tick ( 1 .. $yticks ) {
    $pixmapChart->draw_line(
      $red_gc,
      $margin + $axemargin + ( $tick * $tickstep ),
      $height - $margin - $axemargin,
      $margin + $axemargin + ( $tick * $tickstep ),
      $height - $margin - $axemargin + $tickwidth
    );
  }

  $range = $width - 2 * $margin - 2 * $axemargin;
  $center = $height / 2;
  $old = $center;
  for $step ( 1 .. $range ) {
    $new = 3.14 / ( $range / 2 - $step + 0.000000001 ) * 20 + $center;
    if ( $new > ( $height - $margin - $axemargin ) ) {
      $new = $margin + $axemargin;
    }
    $pixmapChart->draw_line(
      $green_gc,
      $margin + $axemargin + $step,
      $old,
      $margin + $axemargin + $step + 1,
      $new
    );
    $old = $new;
  }

}

# main

  my (
    $windowMain,
    $vboxMain,
    $dareaChart
  );

  $windowMain = new Gtk::Window( 'toplevel' );

  $windowMain->signal_connect( 'destroy', sub { Gtk::main_quit( Gtk ); } );

  $windowMain->set_title( 'Chart drawing' );
  $windowMain->border_width( 0 );

  $vboxMain = new Gtk::VBox ( 0, 0 );
  $vboxMain->border_width( 10 );
  $windowMain->add( $vboxMain );
  $vboxMain->show;

  $dareaChart = new Gtk::DrawingArea;
  $dareaChart->signal_connect( 'expose_event', \&do_chart_expose );
  $dareaChart->size( $width, $height );
  $vboxMain->pack_start( $dareaChart, 1, 1, 0 );
  $dareaChart->set_events( [ 'exposure_mask' ] );
  $dareaChart->show;

  $dareaChart->realize;

  draw_chart ( $dareaChart );

  $windowMain->show;

  Gtk::main( Gtk );

# end
